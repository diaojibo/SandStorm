//
// Created by rocklct on 2/15/18.
//

#pragma once

#include "sandstorm/message/CommandServer.h"
#include "sandstorm/util/NetListener.h"
#include "sandstorm/base/NetAddress.h"
#include "sandstorm/service/ManagerContext.h"
#include "sandstorm/collector/OutputDispatcher.h"

#include <functional>
#include <memory>

namespace sandstorm {
    namespace util {
        class NetConnector;

        class Configuration;
    }

    namespace message {
        class CommandClient;
    }

    namespace task {
        class SpoutExecutor;

        class BoltExecutor;
    }

    namespace topology {
        class Topology;
    }

    namespace collector {
        class OutputCollector;

        class TaskQueue;
    }

    namespace service {
        class Manager : public sandstorm::message::CommandServer<ManagerContext> {
        public:
            static void SetGlobalInstance(Manager *manager);

            static Manager *GetGlobalInstance();

            typedef std::function<void(const sandstorm::message::Response &response)> JoinPresidentCallback;

            Manager(const sandstorm::util::Configuration &configuration);

            void OnConnect(ManagerContext *context);

            void JoinPresident(JoinPresidentCallback callback);

            void OnHeartbeat(ManagerContext *context, const sandstorm::message::Command &command,
                             sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            void OnSyncMetadata(ManagerContext *context, const sandstorm::message::Command &command,
                                sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            void OnSendTuple(ManagerContext *context, const sandstorm::message::Command &command,
                             sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            message::CommandClient *CreatePresidentClient();

            std::string GetTopologyName() const;

        private:
            void InitSelfContext();

            void InitExecutors();

            void OwnManagerTasks();

            void ShowManagerMetadata();

            void ShowTaskInfos();

            void InitSpoutExecutors();

            void InitBoltExecutors();

            void InitPresidentConnector();

            void ReserveExecutors();

            void InitEvents();

            void InitTaskFieldsMap();

        private:
            std::string _name;
            std::string _host;
            int32_t _port;
            std::shared_ptr<sandstorm::util::Configuration> _managerConfiguration;
            sandstorm::util::NetConnector *_presidentConnector;
            sandstorm::message::CommandClient *_presidentClient;
            std::shared_ptr<sandstorm::service::ManagerContext> _selfContext;
            std::vector<std::shared_ptr<sandstorm::task::SpoutExecutor>> _spoutExecutors;
            std::vector<std::shared_ptr<sandstorm::task::BoltExecutor>> _boltExecutors;
            std::vector<std::shared_ptr<sandstorm::collector::OutputCollector>> _spoutCollectors;
            std::vector<std::shared_ptr<sandstorm::collector::OutputCollector>> _boltCollectors;
            std::vector<std::shared_ptr<sandstorm::collector::TaskQueue>> _boltTaskQueues;
            std::shared_ptr<topology::Topology> _topology;
            sandstorm::collector::OutputDispatcher _outputDispatcher;
            std::map<std::string, const std::vector<std::string> *> _taskFields;
            std::map<std::string, const std::map<std::string, int32_t> *> _taskFieldsMap;
        };
    }

}

