#pragma once
#include"headers.h"


class Handler {
private:
	string userName;
	bool accResult;

public:
	Handler();

	bool TryLogin();
	void sendUser(string userName, string userPW);

};