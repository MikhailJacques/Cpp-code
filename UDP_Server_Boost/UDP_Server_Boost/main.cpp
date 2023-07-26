//#include <boost/asio.hpp>
//#include <spdlog/spdlog.h>
//#include <spdlog/sinks/basic_file_sink.h>
//#include <spdlog/sinks/stdout_color_sinks.h>
//#include <thread>
//#include <deque>
//#include <condition_variable>
//#include <exception>
//#include <iostream>
//
//using boost::asio::ip::udp;
//using MessageQueue = std::deque<std::pair<std::string, udp::endpoint>>;
//constexpr int max_length = 1024;
//
//class Server
//{
//public:
//
//    Server(boost::asio::io_context& io_context, short port)
//        : io_context_(io_context),
//        socket_(io_context, udp::endpoint(udp::v4(), port)),
//        work_(boost::asio::make_work_guard(io_context))
//    {
//        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//
//        // The second parameter of basic_file_sink_mt is a boolean that indicates whether the file should be truncated when opened. 
//        // By setting this parameter to false, the file will be opened for appending, and previous logs will not be removed when the program is run again.
//        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("server.log", false);
//
//        // logger with 2 sinks
//        logger_ = std::make_shared<spdlog::logger>("UDP_Server", spdlog::sinks_init_list { console_sink, file_sink });
//        logger_->set_level(spdlog::level::trace);
//        spdlog::register_logger(logger_);
//        start_receive();
//        process_messages_thread_ = std::thread([this] { process_messages(); });
//    }
//
//    ~Server()
//    {
//        stop();
//        spdlog::drop_all();
//    }
//
//    void stop()
//    {
//        running_ = false;
//        cond_var_.notify_one();
//        if (process_messages_thread_.get_id() != std::this_thread::get_id())
//        {
//            process_messages_thread_.join();
//        }
//        work_.reset();
//        io_context_.stop();
//
//        logger_->info("Server has stopped.\n");
//        std::cout << "Server has stopped.\n";
//    }
//
//private:
//
//    void start_receive()
//    {
//        socket_.async_receive_from(
//            boost::asio::buffer(data_, max_length), sender_endpoint_,
//            [this](boost::system::error_code ec, std::size_t bytes_rec)
//            {
//                if (!ec && bytes_rec > 0)
//                {
//                    std::lock_guard<std::mutex> guard(mutex_);
//                    message_queue_.emplace_back(std::string(data_, bytes_rec), sender_endpoint_);
//                    cond_var_.notify_one();
//                }
//                else if (ec)
//                {
//                    logger_->error("Receive error: {}", ec.message());
//                }
//
//                if (running_)
//                {
//                    // Start receiving next message
//                    start_receive();
//                }
//            });
//    }
//
//
//    void process_messages()
//    {
//        while (running_ || !message_queue_.empty())
//        {
//            std::pair<std::string, udp::endpoint> message;
//            {
//                std::unique_lock<std::mutex> lock(mutex_);
//                cond_var_.wait(lock, [this] { return !message_queue_.empty() || !running_; });
//
//                if (!running_ && message_queue_.empty())
//                {
//                    break;
//                }
//
//                message = std::move(message_queue_.front());
//                message_queue_.pop_front();
//            }
//
//            if (message.first == "quit")
//            {
//                stop();
//                break;
//            }
//
//            logger_->info("Received: {}", message.first);
//
//            auto response = std::make_shared<std::string>("Message received: " + message.first);
//            socket_.async_send_to(boost::asio::buffer(*response), message.second,
//                [this, response](boost::system::error_code ec, std::size_t)
//                {
//                    if (ec)
//                    {
//                        logger_->error("Send error: {}", ec.message());
//                    }
//                });
//        }
//    }
//
//    udp::socket socket_;
//    udp::endpoint sender_endpoint_;
//    char data_[max_length];
//    MessageQueue message_queue_;
//    std::mutex mutex_;
//    std::condition_variable cond_var_;
//    std::thread process_messages_thread_;
//    std::shared_ptr<spdlog::logger> logger_;
//    bool running_ = true;
//    boost::asio::io_context& io_context_;
//    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
//};
//
//int main()
//{
//    try
//    {
//        boost::asio::io_context io_context;
//        Server server(io_context, 1313);
//        std::cout << "Server started. Listening on port 1313...\n";
//        io_context.run();
//    }
//    catch (std::exception& e)
//    {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
//
//    return 0;
//}