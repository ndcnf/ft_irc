// #include "Socket.hpp" // NADIA
#include "Server.hpp" // NADIA

// mode d'emploi : ./ircserv <port> <password>
int	main(int argc, char *argv[])
{
	// NADIA
	try
	{
		// [ ] Gestion d'erreur ici a ajouter lors du parsing

		// [X] SOCKET()
		// [X] SETSOCKOPT()
		// [X] BIND()
		// [X] LISTEN()

		// [ ] POLL() / SELECT()
		// [ ] ACCEPT()
		// [ ] SEND()
		// [ ] CLOSE()

		Server	srv;
		if (argc < 3)
			throw (std::exception());

		srv.setPort(atoi(argv[1]));
		std::cout << "PORT: " << srv.getPort() << std::endl; // DEBUG ONLY
		srv.createSocket();
		// srv.createSocket();
		// close(srv.getSocket());
		// srv.createSocket();

		srv.allSockets(); // DEBUG ONLY

		// -------- tests

		struct sockaddr_in	addr;

		bzero(&addr, sizeof(addr));
		int fd_client;
		unsigned int len;
		char buf[250];
		int ret;
		char basic[] = "je suis arrive";

		while (true)
		{ 
			len = sizeof(addr);
			fd_client = accept(srv.getSocket(), (struct sockaddr *)&addr, &len);
			if (fd_client > 0)
			{
				std::cout << "client accepte !" << std::endl;
				std::cout << inet_ntoa(addr.sin_addr) << std::endl; //affichage de l'adresse en decimal du client
				ret = recv(fd_client, &buf, sizeof(buf), 0);
				write(1, buf, ret);
				send(fd_client, basic, sizeof(basic), 0);

			}
			// else
			// {
			// 	std::cout << "dommage" << std::endl;
			// }
		}

		// ------ fin tests

		return (0);
	}
	catch(const std::exception& e)
	{
		// std::cerr << errno << '\n';
		std::cerr << e.what() << '\n';
	}











	// std::cout	<< error << std::endl;
	/* NADIA



	*/

	// CLAIRE

	/* CLAIRE

	*/

	// VERENA

	/* VERENA

	*/



	// return (0);
}
