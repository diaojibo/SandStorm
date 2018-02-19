//
// Created by rocklct on 2/18/18.
//

#include "sandstorm/spout/SpoutDeclarer.h"
#include "sandstorm/spout/ISpout.h"

namespace sandstorm {
    namespace spout {
        SpoutDeclarer::SpoutDeclarer(const std::string &spoutName, ISpout *spout) :
                _spout(spout) {
            SetType(sandstorm::task::TaskDeclarer::Type::Spout);
            SetTaskName(spoutName);

            _fields = _spout->DeclareFields();
            int32_t fieldIndex = 0;
            for (const std::string &field : _fields) {
                _fieldsMap.insert({field, fieldIndex});
            }
        }
    }
}
