//
// Created by rocklct on 2/15/18.
//


#pragma once

#include "sandstorm/message/CommandServer.h"
#include "sandstorm/util/NetListener.h"
#include "sandstorm/base/NetAddress.h"
#include "sandstorm/service/ManagerContext.h"

#include <thread>

namespace sandstorm {
    namespace util {
        class Configuration;
    }

    namespace topology {
        class Topology;
    }

    namespace message {
        class CommandClient;
    }

    namespace spout {
        class SpoutDeclarer;
    }

    namespace bolt {
        class BoltDeclarer;
    }

    namespace service {
        typedef std::pair<std::string, std::string> TaskPathName;

        class President : public sandstorm::message::CommandServer<ManagerContext> {
        public:
            President(const sandstorm::base::NetAddress &host);

            President(const sandstorm::util::Configuration &configuration);

            void OnConnect(ManagerContext *context);

            void OnJoin(ManagerContext *context, const sandstorm::message::Command &command,
                        sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            void OnAskField(ManagerContext *context, const sandstorm::message::Command &command,
                            sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            void OnOrderId(ManagerContext *context, const sandstorm::message::Command &command,
                           sandstorm::message::CommandServer<sandstorm::message::BaseCommandServerContext>::Responsor Responsor);

            void SubmitTopology(sandstorm::topology::Topology *topology);

        private:
            void SendHeartbeat(const std::string managerId, int32_t sendTimes);

            std::list<sandstorm::task::TaskInfo>
            GetAllSpoutTasks(const std::map<std::string, sandstorm::spout::SpoutDeclarer> &spoutDeclarers,
                             sandstorm::topology::Topology *topology);

            void AllocateSpoutTasks(std::map<std::string, sandstorm::task::TaskInfo *> nameToSpoutTasks,
                                    std::list<sandstorm::task::TaskInfo> originSpoutTasks);

            std::map<std::string, std::vector<task::TaskInfo *> >
            AllocateSpoutTasks(std::list<task::TaskInfo> &originSpoutTasks);

            std::list<sandstorm::task::TaskInfo> GetAllBoltTasks(sandstorm::topology::Topology *topology,
                                                                 const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers);

            std::map<std::string, std::vector<task::TaskInfo *> >
            AllocateBoltTasks(std::list<task::TaskInfo> &originBoltTasks);

            std::vector<task::TaskInfo *> FindTask(
                    const std::map<std::string, std::vector<task::TaskInfo *> > &nameToBoltTasks,
                    const std::map<std::string, std::vector<task::TaskInfo *> > &nameToSpoutTasks,
                    const std::string &sourceTaskName);

            std::vector<task::TaskInfo *> FindTask(
                    const std::map<std::string, std::vector<task::TaskInfo *> > &nameToBoltTasks,
                    const std::string &sourceTaskName);

            void CalculateTaskPaths(
                    const std::map<std::string, std::vector<task::TaskInfo *> > &nameToBoltTasks,
                    const std::map<std::string, sandstorm::bolt::BoltDeclarer> &boltDeclarers,
                    const std::map<std::string, std::vector<task::TaskInfo *> > &nameToSpoutTasks);

            void ShowManagerMetadata();

            void ShowManagerTaskInfos();

            void ShowTaskInfos(const std::vector<sandstorm::task::TaskInfo> &taskInfos);

            void SyncWithManagers();

            void StartConnectionChecker();

            void CheckConnections();

        private:
            sandstorm::base::NetAddress _presidentHost;
            std::vector<ManagerContext> _managers;
            int32_t _managerCount;
            std::shared_ptr<sandstorm::util::Configuration> _configuration;
            std::map<std::string, std::shared_ptr<sandstorm::message::CommandClient>> _managerClients;
            std::map<TaskPathName, std::vector<task::ExecutorPosition>> _fieldsCandidates;
            std::map<TaskPathName, std::map<std::string, task::ExecutorPosition>> _fieldsDestinations;
            std::map<std::string, int64_t> _orderIds;
            bool _submitted;

            std::thread _connectionChecker;
        };
    }
}
