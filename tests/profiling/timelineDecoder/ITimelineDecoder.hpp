//
// Copyright © 2020 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <cstdint>
#include <string>

class ITimelineDecoder
{

public:

    enum class ErrorCode
    {
        ErrorCode_Success,
        ErrorCode_Fail
    };

    enum class RelationshipType
    {
        RetentionLink, /// Head retains(parents) Tail
        ExecutionLink, /// Head execution start depends on Tail execution completion
        DataLink,      /// Head uses data of Tail
        LabelLink      /// Head uses label Tail (Tail MUST be a guid of a label).
    };

    struct Entity
    {
        uint64_t m_Guid;
    };

    struct EventClass
    {
        uint64_t m_Guid;
    };

    struct Event
    {
        uint64_t m_Guid;
        uint64_t m_TimeStamp;
        uint64_t m_ThreadId;
    };

    struct Label
    {
        uint64_t m_Guid;
        std::string m_Name;
    };

    struct Relationship
    {
        RelationshipType m_RelationshipType;
        uint64_t m_Guid;
        uint64_t m_HeadGuid;
        uint64_t m_TailGuid;
    };

    virtual ~ITimelineDecoder() = default;

    virtual ErrorCode CreateEntity(const Entity&) = 0;
    virtual ErrorCode CreateEventClass(const EventClass&) = 0;
    virtual ErrorCode CreateEvent(const Event&) = 0;
    virtual ErrorCode CreateLabel(const Label&) = 0;
    virtual ErrorCode CreateRelationship(const Relationship&) = 0;
};