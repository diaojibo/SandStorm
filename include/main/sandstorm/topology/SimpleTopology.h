//
// Created by rocklct on 1/11/18.
//


#pragma once

#include "sandstorm/topology/ITopology.h"
#include "sandstorm/spout/SpoutExecutor.h"
#include "sandstorm/bolt/BoltExecutor.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

namespace sandstorm {
    namespace topology {
        class SimpleTopology : public ITopology {
        public:
            SimpleTopology();

            // Inherited via ITopology

            void SetSpouts(std::map<std::string, std::shared_ptr<spout::ISpout>> spouts) override;

            void SetBolts(std::map<std::string, std::shared_ptr<bolt::IBolt>> bolts) override;

            void SetNetWork(std::map<std::string, std::vector<std::string>> network) override;

            void EmitFrom(const std::string &src, const base::Values &values) override;

            void Start() override;

            virtual const std::map<std::string, std::shared_ptr<spout::ISpout>> &GetSpouts() const override;

            virtual const std::map<std::string, std::shared_ptr<bolt::IBolt>> &GetBolts() const override;

            virtual const std::map<std::string, std::vector<std::string>> &GetNetwork() const override;


        private:
            template<class TaskType, class ExecutorType>
            void allocateTask(std::map<std::string, std::shared_ptr<TaskType>> &task,
                              std::vector<std::shared_ptr<ExecutorType>> &freeExecutors,
                              std::map<std::string, std::vector<std::shared_ptr<ExecutorType>>> &allocatedExecutor
            );

            std::map<std::string, std::shared_ptr<spout::ISpout>> _spouts;
            std::map<std::string, std::shared_ptr<bolt::IBolt>> _bolts;
            std::map<std::string, std::vector<std::string>> _network;

        };
    }
}
