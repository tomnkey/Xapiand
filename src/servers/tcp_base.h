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

#include "../manager.h"

#include <iostream>
#include <string>

class XapiandManager;

// Values in seconds.
constexpr double idle_timeout = 60;
constexpr double active_timeout = 15;


// Base class for configuration data for TCP.
class BaseTCP {
private:
	void bind(int tries);
	void check_backlog(int backlog);

protected:
	std::shared_ptr<XapiandManager> manager;

	int port;
	int sock;

	std::string description;

public:
	BaseTCP(std::shared_ptr<XapiandManager>&& manager_, int port_, const std::string &description_, int tries_);
	virtual ~BaseTCP();

	virtual std::string getDescription() const noexcept = 0;

	int accept();

	static int connect(int sock_, const std::string &hostname, const std::string &servname);
};
