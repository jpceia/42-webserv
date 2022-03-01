#include "includes/MultiClientChat.hpp"

int main(int argc, char **argv) {

	MultiClientChat		multi_client_chat("0.0.0.0", 54000);

	if (multi_client_chat.init(-1) != 0) {
		return 1;
	}

	multi_client_chat.run();

	/*while (1)
	{};*/
}
