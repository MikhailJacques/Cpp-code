// The code is a basic UDP server that accepts messages from a client and echoes to it the received message.
// The client operates in a loop until the user types 'quit'.
// It uses Boost's Asio library for network I/O and spdlog for logging.

// Boost Asio is a cross-platform C++ library for network programming
#include <boost/asio.hpp>

// spdlog is a very fast, header-only/compiled, C++ logging library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <thread>
#include <deque>
#include <condition_variable>
#include <exception>
#include <iostream>

// Define a type alias for the udp protocol
using boost::asio::ip::udp;

// Define a type alias for the queue which holds the messages
using MessageQueue = std::deque<std::pair<std::string, udp::endpoint>>;

// Define the maximum length of a message
constexpr int max_length = 1024;

class Server
{
public:

    // constructor
    Server(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
        
        // initializing the socket
        socket_(io_context, udp::endpoint(udp::v4(), port)),    
        
        // creating a work object to keep the io_context object alive
        work_(boost::asio::make_work_guard(io_context))         
    {
        // creating a sink which logs to the console
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // creating a sink which logs to a file
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("server.log", false);

        // creating a logger which logs to both the console and a file
        logger_ = std::make_shared<spdlog::logger>("UDP_Server", spdlog::sinks_init_list { console_sink, file_sink });

        // setting the level of the logger to trace which means all messages will be logged
        logger_->set_level(spdlog::level::trace);

        // registering the logger
        spdlog::register_logger(logger_);

        // starting the receive loop
        start_receive();

        // starting the message processing thread
        process_messages_thread_ = std::thread([this] { process_messages(); });
    }

    // destructor
    ~Server()
    {
        // stopping the server
        stop();

        // unregistering all loggers
        spdlog::drop_all();
    }

    // function to stop the server
    void stop()
    {
        // stopping the receive loop
        running_ = false;

        // notifying the message processing thread
        cond_var_.notify_one();

        // joining the message processing thread if it's not the current thread
        if (process_messages_thread_.get_id() != std::this_thread::get_id())
        {
            process_messages_thread_.join();
        }

        // stopping the io_context object
        work_.reset();
        io_context_.stop();

        // logging that the server has stopped
        logger_->info("Server has stopped.\n");
        std::cout << "Server has stopped.\n";
    }

private:

    // function to start receiving messages
    void start_receive()
    {
        socket_.async_receive_from(

            // specifying the buffer to store the received data
            boost::asio::buffer(data_, max_length), sender_endpoint_,

            // specifying the handler to be called when the receive operation is complete
            [this](boost::system::error_code ec, std::size_t bytes_rec)
            {
                // checking if the receive operation was successful
                if (!ec && bytes_rec > 0)
                {
                    // locking the mutex to protect access to the message queue
                    std::lock_guard<std::mutex> guard(mutex_);

                    // adding the received message to the message queue
                    message_queue_.emplace_back(std::string(data_, bytes_rec), sender_endpoint_);

                    // notifying the message processing thread
                    cond_var_.notify_one();
                }
                // logging any errors that occurred during the receive operation
                else if (ec)
                {
                    logger_->error("Receive error: {}", ec.message());
                }

                // continuing to receive messages if the server is running
                if (running_)
                {
                    // Start receiving next message
                    start_receive();
                }
            });
    }

    // function to process messages
    void process_messages()
    {
        // running the loop while the server is running or there are messages in the queue
        while (running_ || !message_queue_.empty())
        {
            std::pair<std::string, udp::endpoint> message;
            {
                // locking the mutex to protect access to the message queue
                std::unique_lock<std::mutex> lock(mutex_);

                // waiting until there are messages in the queue or the server is not running
                cond_var_.wait(lock, [this] { return !message_queue_.empty() || !running_; });

                // stopping if the server is not running and the queue is empty
                if (!running_ && message_queue_.empty())
                {
                    break;
                }

                // getting the message from the front of the queue and removing it from the queue
                message = std::move(message_queue_.front());
                message_queue_.pop_front();
            }

            // stopping the server if the received message is "quit"
            if (message.first == "quit")
            {
                stop();
                break;
            }

            // logging the received message
            logger_->info("Received: {}", message.first);

            // creating the response message
            auto response = std::make_shared<std::string>("Message received: " + message.first);

            // sending the response message
            socket_.async_send_to(boost::asio::buffer(*response), message.second,

                // specifying the handler to be called when the send operation is complete
                [this, response](boost::system::error_code ec, std::size_t)
                {
                    // logging any errors that occurred during the send operation
                    if (ec)
                    {
                        logger_->error("Send error: {}", ec.message());
                    }
                });
        }
    }

    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    char data_[max_length];
    MessageQueue message_queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::thread process_messages_thread_;
    std::shared_ptr<spdlog::logger> logger_;
    bool running_ = true;
    boost::asio::io_context& io_context_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
};


int main()
{
    try
    {
        // creating the io_context object
        boost::asio::io_context io_context;

        // creating the server object
        Server server(io_context, 1313);
        std::cout << "Server started. Listening on port 1313...\n";

        // starting the io_context event loop
        io_context.run();
    }
    // catching and handling any exceptions that are thrown
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
