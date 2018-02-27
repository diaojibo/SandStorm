//
// Created by rocklct on 1/30/18.
//

namespace sandstorm {
    namespace message {
        template<class CommandServerContext>
        CommandServer<CommandServerContext>::CommandServer(sandstorm::util::NetListener *listener) :
                _listener(listener) {
        }

        template<class CommandServerContext>
        CommandServer<CommandServerContext>::~CommandServer() {
            if (_listener) {
                delete _listener;
                _listener = nullptr;
            }
        }

        template<class CommandServerContext>
        void CommandServer<CommandServerContext>::StartListen() {
            std::map <int32_t, CommandHandler> &commandHandlers = _commandHandlers;
            ConnectHandler connectHandler = _connectHandler;


            // pass all the command handlers into  ConnectionHandler
            _listener->OnConnection([this, connectHandler, commandHandlers](
                    std::shared_ptr <sandstorm::network::TcpConnection> connection) {
                CommandServerContext *context = new CommandServerContext;
                sandstorm::network::TcpConnection *rawConnection = connection.get();

                _connectHandler(context);

                connection->OnData([this, commandHandlers, context, rawConnection]
                                           (const char *buffer, int32_t size) {

                    //translate Command BytesArray(Deserialize)
                    sandstorm::base::ByteArray commandBytes(buffer, size);
                    Command command;
                    command.Deserialize(commandBytes);

                    int32_t commandType = command.GetType();
                    try {
                        CommandHandler handler = commandHandlers.at(commandType);
                        Responsor Responsor = std::bind(&CommandServer::Response, this, rawConnection,
                                                        std::placeholders::_1);

                        handler(context, command, Responsor);
                    }
                    catch (const std::exception &e) {

                        Responsor Responsor = std::bind(&CommandServer::Response, this, rawConnection,
                                                        std::placeholders::_1);

                        sandstorm::message::Response response(sandstorm::message::Response::Status::Failed);
                        Responsor(response);
                    }
                });
            });

            _listener->StartListen();
        }

        template<class CommandServerContext>
        void CommandServer<CommandServerContext>::Response(sandstorm::network::TcpConnection *connection,
                                                           const sandstorm::message::Response &response) {
            sandstorm::base::ByteArray responseBytes = response.Serialize();
            connection->Send(responseBytes.data(), responseBytes.size());
        }


        template<class CommandServerContext>
        void CommandServer<CommandServerContext>::OnCommand(int32_t commandType, CommandHandler handler) {
            _commandHandlers.insert({commandType, handler});
        }

        template<class CommandServerContext>
        void CommandServer<CommandServerContext>::OnConnection(ConnectHandler handler) {
            _connectHandler = handler;
        }


    }

}