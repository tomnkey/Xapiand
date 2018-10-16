/*
 * Copyright (C) 2015-2018 Dubalu LLC. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "xapiand.h"

#ifdef XAPIAND_CLUSTERING

#include "base_udp.h"
#include "endpoint.h"


// Values in seconds
constexpr double WAITING_FAST  = 0.200;
constexpr double WAITING_SLOW  = 0.600;
constexpr double HEARTBEAT_MIN = 1.500;
constexpr double HEARTBEAT_MAX = 3.000;
constexpr double NODE_LIFESPAN = 2.5 * HEARTBEAT_MAX;

constexpr uint16_t XAPIAND_DISCOVERY_PROTOCOL_MAJOR_VERSION = 1;
constexpr uint16_t XAPIAND_DISCOVERY_PROTOCOL_MINOR_VERSION = 0;

constexpr uint16_t XAPIAND_DISCOVERY_PROTOCOL_VERSION = XAPIAND_DISCOVERY_PROTOCOL_MAJOR_VERSION | XAPIAND_DISCOVERY_PROTOCOL_MINOR_VERSION << 8;


// Discovery for nodes and databases
class Discovery : public UDP, public Worker {
	enum class Message {
		HEARTBEAT,     // Heartbeat
		HELLO,         // New node saying hello
		WAVE,          // Nodes waving hello to the new node
		SNEER,         // Nodes telling the client they don't agree on the new node's name
		ENTER,         // Node enters the room
		BYE,           // Node says goodbye
		DB_UPDATED,    //
		MAX,           //
	};

	static const std::string& MessageNames(Message type) {
		static const std::string MessageNames[] = {
			"HEARTBEAT", "HELLO", "WAVE", "SNEER", "ENTER", "BYE", "DB_UPDATED",
		};

		auto type_int = static_cast<int>(type);
		if (type_int >= 0 || type_int < static_cast<int>(Message::MAX)) {
			return MessageNames[type_int];
		}
		static const std::string UNKNOWN = "UNKNOWN";
		return UNKNOWN;
	}

	ev::io io;
	ev::timer node_heartbeat;

	void send_message(Message type, const std::string& message);
	void io_accept_cb(ev::io& watcher, int revents);
	void discovery_server(Discovery::Message type, const std::string& message);

	void heartbeat(const std::string& message);
	void hello(const std::string& message);
	void wave(const std::string& message);
	void sneer(const std::string& message);
	void enter(const std::string& message);
	void bye(const std::string& message);
	void db_updated(const std::string& message);

	void heartbeat_cb(ev::timer& watcher, int revents);

	void destroyer();

	void destroy_impl() override;
	void shutdown_impl(time_t asap, time_t now) override;

public:
	Discovery(const std::shared_ptr<Worker>& parent_, ev::loop_ref* ev_loop_, unsigned int ev_flags_, int port_, const std::string& group_);
	~Discovery();

	void start();
	void stop();

	void signal_db_update(const Endpoint& endpoint);

	std::string __repr__() const override {
		return Worker::__repr__("Discovery");
	}

	std::string getDescription() const noexcept override;
};

#endif