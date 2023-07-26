//#include <boost/asio.hpp>
//#include <spdlog/spdlog.h>
//#include <spdlog/sinks/basic_file_sink.h>
//#include <iostream>
//
//using boost::asio::ip::udp;
//
//class Client
//{
//    public:
//
//        Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
//            : io_context_(io_context), socket_(io_context, udp::endpoint(udp::v4(), 0)),
//            logger_(spdlog::basic_logger_mt("basic_logger", "client.log"))
//        {
//            udp::resolver resolver(io_context_);
//            receiver_endpoint_ = *resolver.resolve(udp::v4(), host, port).begin();
//            logger_->set_level(spdlog::level::trace);  // Or whatever minimum level you want.
//
//        }
//
//        void run()
//        {
//            while (true)
//            {
//                std::cout << "Enter message (or 'quit' to exit): ";
//                std::string input;
//                std::getline(std::cin, input);
//
//                socket_.send_to(boost::asio::buffer(input), receiver_endpoint_);
//                logger_->info("Sent: {}", input);  // Log the sent message.
//
//                if (input == "quit")
//                    break;
//
//                char reply[max_length];
//                udp::endpoint sender_endpoint;
//                size_t reply_length = socket_.receive_from(boost::asio::buffer(reply, max_length), sender_endpoint);
//                std::cout << "Reply is: ";
//                std::cout.write(reply, reply_length);
//                std::cout << "\n";
//                logger_->info("Received: {}", std::string(reply, reply_length));  // Log the received reply.
//            }
//        }
//
//    private:
//
//        boost::asio::io_context& io_context_;
//        udp::socket socket_;
//        std::shared_ptr<spdlog::logger> logger_;
//        udp::endpoint receiver_endpoint_;
//
//        static constexpr size_t max_length = 1024;
//};
//
//int main(int argc, char* argv[])
//{
//    if (argc != 3)
//    {
//        std::cerr << "Usage: <host> <port>\n";
//        return 1;
//    }
//
//    boost::asio::io_context io_context;
//    Client client(io_context, argv[1], argv[2]);
//    client.run(); // Run the client
//    return 0;
//}