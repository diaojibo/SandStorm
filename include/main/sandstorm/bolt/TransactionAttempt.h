//
// Created by rocklct on 1/28/18.
//

#pragma once

namespace sandstorm {
    namespace bolt {
        class TransactionAttempt {
        public:
            TransactionAttempt(int32_t transactionId, int32_t attemptId) :
                    _transactionId(transactionId), _attemptId(attemptId) {}

            int32_t GetTransactionId() const {
                return _transactionId;
            }

            int32_t GetAttemptId() const {
                return _attemptId;
            }

        private:
            int32_t _transactionId;
            int32_t _attemptId;
        };
    }
}
