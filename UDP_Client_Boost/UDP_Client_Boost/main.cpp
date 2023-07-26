// The code is a basic UDP client that sends messages to a server and logs the sent messages and received replies.
// The client operates in a loop until the user types 'quit'.
// It uses Boost's Asio library for network I/O and spdlog for logging.

// Include the necessary header files
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>

// Import the udp namespace from boost::asio::ip for convenience
using boost::asio::ip::udp;

class Client
{
public:

    // Constructor takes an IO context and strings for host and port
    // Initialize the IO context, socket, and logger here
    Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), 0)),
        logger_(spdlog::basic_logger_mt("basic_logger", "client.log"))
    {
        // Resolve the server's endpoint and store it for later use
        udp::resolver resolver(io_context_);
        receiver_endpoint_ = *resolver.resolve(udp::v4(), host, port).begin();

        // Set the logging level
        logger_->set_level(spdlog::level::trace);
    }

    // The run function handles sending and receiving messages in a loop
    void run()
    {
        while (true)
        {
            std::cout << "Enter message (or 'quit' to exit): ";
            std::string input;
            std::getline(std::cin, input);  // Get user input

            // Send the input to the server
            socket_.send_to(boost::asio::buffer(input), receiver_endpoint_);

            // Log the sent message
            logger_->info("Sent: {}", input);

            // Exit the loop if the user inputs 'quit'
            if (input == "quit")  
                break;

            // Prepare to receive reply
            char reply[max_length];
            udp::endpoint sender_endpoint;
            size_t reply_length = socket_.receive_from(boost::asio::buffer(reply, max_length), sender_endpoint);

            // Output the reply
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";

            // Log the received reply
            logger_->info("Received: {}", std::string(reply, reply_length));
        }
    }

private:
    // Declare class members
    boost::asio::io_context& io_context_;
    udp::socket socket_;
    std::shared_ptr<spdlog::logger> logger_;
    udp::endpoint receiver_endpoint_;
    static constexpr size_t max_length = 1024;
};

// Entry point for the program
int main(int argc, char* argv[])
{
    // Check for correct number of command-line arguments
    if (argc != 3)
    {
        std::cerr << "Usage: <host> <port>\n";
        return 1;
    }

    // Initialize the IO context
    boost::asio::io_context io_context;

    // Initialize the client with the IO context, host, and port
    Client client(io_context, argv[1], argv[2]);

    // Run the client
    client.run();

    return 0;
}