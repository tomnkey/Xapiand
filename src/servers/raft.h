/*
 * Copyright (C) 2015 deipi.com LLC and contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "udp_base.h"

#include "server_raft.h"

#define ELECTION_LEADER_MIN	4.0 * HEARTBEAT_MAX
#define ELECTION_LEADER_MAX	4.5 * HEARTBEAT_MAX

#define XAPIAND_RAFT_PROTOCOL_MAJOR_VERSION 1
#define XAPIAND_RAFT_PROTOCOL_MINOR_VERSION 0

constexpr uint16_t XAPIAND_RAFT_PROTOCOL_VERSION = XAPIAND_RAFT_PROTOCOL_MAJOR_VERSION | XAPIAND_RAFT_PROTOCOL_MINOR_VERSION << 8;


// The Raft consensus algorithm
class Raft : public BaseUDP {
private:
	uint64_t term;
	size_t votes;
	size_t num_servers;

	ev::timer election_leader;
	ev::tstamp election_timeout;

	ev::timer heartbeat;
	ev::tstamp last_activity;

	std::string votedFor;
	std::string leader;

	void leader_election_cb(ev::timer &watcher, int revents);
	void heartbeat_cb(ev::timer &watcher, int revents);

	void start_heartbeat();

	friend RaftServer;

public:
	enum class State {
		LEADER,
		FOLLOWER,
		CANDIDATE
	};

	enum class Message {
		REQUEST_VOTE,       // Invoked by candidates to gather votes
		RESPONSE_VOTE,      // Gather votes
		HEARTBEAT_LEADER,   // Only leader send heartbeats to its follower servers
		LEADER,             // Node saying hello when it become leader
		REQUEST_DATA,       // Request information from leader
		RESPONSE_DATA,      // Receive information from leader
		RESET,              // Force reset a node
	};

	State state;

	Raft(std::shared_ptr<XapiandManager>&& manager_, ev::loop_ref *loop_, int port_, const std::string &group_);
	~Raft();

	void reset();

	void send_message(Message type, const std::string &content);

	void register_activity();

	std::string getDescription() const noexcept override;

	inline void start() {
		election_leader.start();
	}
};
