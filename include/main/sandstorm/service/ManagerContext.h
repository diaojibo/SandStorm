//
// Created by rocklct on 2/15/18.
//

# pragma once

#include "sandstorm/task/TaskInfo.h"

#include <string>
#include <set>
#include <vector>

namespace sandstorm {
    namespace service {
        class ManagerContext : base::Serializable {
        public:
            enum class ExecutorType {
                Spout,
                Bolt
            };

            ManagerContext();


            const std::string &GetId() const {
                return _id;
            }

            void SetId(const std::string &id) {
                _id = id;
            }

            const sandstorm::base::NetAddress &GetNetAddress() const {
                return _netAddress;
            }

            void SetNetAddress(const sandstorm::base::NetAddress &netAddress) {
                _netAddress = netAddress;
            }

            int32_t GetSpoutCount() const {
                return _spoutCount;
            }

            void SetSpoutCount(int32_t spoutCount) {
                _spoutCount = spoutCount;
            }

            int32_t GetBoltCount() const {
                return _boltCount;
            }

            void SetBoltCount(int32_t boltCount) {
                _boltCount = boltCount;
            }

            const std::set<int32_t> &GetFreeSpouts() const {
                return _freeSpouts;
            }

            void SetFreeSpouts(const std::set<int32_t> &freeSpouts) {
                _freeSpouts = freeSpouts;
            }

            const std::set<int32_t> &GetBusySpouts() const {
                return _busySpouts;
            }

            void SetBusySpouts(const std::set<int32_t> &busySpouts) {
                _busySpouts = busySpouts;
            }

            const std::set<int32_t> &GetFreeBolts() const {
                return _freeBolts;
            }

            void SetFreeBolts(const std::set<int32_t> &freeBolts) {
                _freeBolts = freeBolts;
            }

            const std::set<int32_t> &GetBusyBolts() const {
                return _busyBolts;
            }

            void SetBusyBolts(const std::set<int32_t> &busyBolts) {
                _busyBolts = busyBolts;
            }

            void useSpout(int32_t spoutIndex) {
                _freeSpouts.erase(spoutIndex);
                _busySpouts.insert(spoutIndex);
            }


            int32_t useNextSpout() {
                if (!_freeSpouts.size()) {
                    return -1;
                }

                int32_t spoutIndex = *(_freeSpouts.begin());
                useSpout(spoutIndex);

                return spoutIndex;
            }

            void freeSpout(int32_t spoutIndex) {
                _freeSpouts.insert(spoutIndex);
                _busySpouts.erase(spoutIndex);
            }

            void useBolt(int32_t boltIndex) {
                _freeBolts.erase(boltIndex);
                _busyBolts.insert(boltIndex);
            }


            int32_t useNextBolt() {
                if (!_freeBolts.size()) {
                    return -1;
                }

                int32_t boltIndex = *(_freeBolts.begin());
                useBolt(boltIndex);

                return boltIndex;
            }

            void freeBolt(int32_t boltIndex) {
                _freeBolts.insert(boltIndex);
                _busyBolts.erase(boltIndex);
            }


            void PrepareTaskInfos() {
                _taskInfos.resize(_spoutCount + _boltCount);
            }

            std::vector<sandstorm::task::TaskInfo> &GetTaskInfos() {
                return _taskInfos;
            }

            const std::vector<sandstorm::task::TaskInfo> &GetTaskInfos() const {
                return _taskInfos;
            }

            void SetTaskInfos(const std::vector<sandstorm::task::TaskInfo> &taskInfos) {
                _taskInfos = taskInfos;
            }

            const sandstorm::task::TaskInfo &GetTaskInfo(int32_t index) const {
                return _taskInfos[index];
            }

            const sandstorm::task::TaskInfo &GetSpoutTaskInfo(int32_t index) const {
                return _taskInfos[index];
            }

            const sandstorm::task::TaskInfo &GetBoltTaskInfo(int32_t index) const {
                return _taskInfos[_spoutCount + index];
            }

            sandstorm::task::TaskInfo &GetTaskInfo(int32_t index) {
                return _taskInfos[index];
            }

            sandstorm::task::TaskInfo &GetSpoutTaskInfo(int32_t index) {
                return _taskInfos[index];
            }

            sandstorm::task::TaskInfo &GetBoltTaskInfo(int32_t index) {
                return _taskInfos[_spoutCount + index];
            }

            void SetTaskInfo(int32_t index, const sandstorm::task::TaskInfo &info) {
                _taskInfos[index] = info;
            }

            int32_t SetSpoutTaskInfo(int32_t index, const sandstorm::task::TaskInfo &info) {
                _taskInfos[index] = info;

                return index;
            }

            int32_t SetBoltTaskInfo(int32_t index, const sandstorm::task::TaskInfo &info) {
                _taskInfos[_spoutCount + index] = info;

                return _spoutCount + index;
            }

            int32_t GetExecutorIndex(ExecutorType type, int32_t subIndex) {
                if (type == ExecutorType::Spout) {
                    return subIndex;
                }

                return _spoutCount + subIndex;
            }

            virtual void Serialize(sandstorm::base::Variants &variants) const override;

            virtual void Deserialize(sandstorm::base::Variants::const_iterator &it) override;


        private:
            std::string _id;
            sandstorm::base::NetAddress _netAddress;

            int32_t _spoutCount;
            int32_t _boltCount;

            std::set<int32_t> _freeSpouts;
            std::set<int32_t> _freeBolts;

            std::set<int32_t> _busySpouts;
            std::set<int32_t> _busyBolts;

            std::vector<sandstorm::task::TaskInfo> _taskInfos;
        };
    }
}
