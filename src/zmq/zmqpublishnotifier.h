// Copyright (c) 2015-2016 The Bitcoin Core developers
// Copyright (c) 2016-2018 The CellLink Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CELLLINK_ZMQ_ZMQPUBLISHNOTIFIER_H
#define CELLLINK_ZMQ_ZMQPUBLISHNOTIFIER_H

#include "zmq/zmqabstractnotifier.h"

class CellBlockIndex;

class CZMQAbstractPublishNotifier : public CZMQAbstractNotifier
{
private:
    uint32_t nSequence; //!< upcounting per message sequence number

public:

    /* send zmq multipart message
       parts:
          * command
          * data
          * message sequence number
    */
    bool SendMessage(const char *command, const void* data, size_t size);

    bool Initialize(void *pcontext) override;
    void Shutdown() override;
};

class CZMQPublishHashBlockNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyBlock(const CellBlockIndex *pindex) override;
};

class CZMQPublishHashTransactionNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyTransaction(const CellTransaction &transaction) override;
};

class CZMQPublishRawBlockNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyBlock(const CellBlockIndex *pindex) override;
};

class CZMQPublishRawTransactionNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyTransaction(const CellTransaction &transaction) override;
};

#endif // CELLLINK_ZMQ_ZMQPUBLISHNOTIFIER_H