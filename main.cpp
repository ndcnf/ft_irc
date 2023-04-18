#include "Socket.hpp" // NADIA
#include "Server.hpp" // NADIA

// mode d'emploi : ./ircserv <port> <password>
int	main(int argc, char *argv[])
{
	// NADIA
	try
	{
		//gestion d'erreur ici a ajouter lors du parsing
		// (void)argc;
		// (void)argv;


		// int	tempura;
		// struct addrinfo addr;


		//------------------------- SOCKET() & SETSOCKOPT() -------------------------//
		// int	error = errno;
		// Socket	sock;
		// std::cout << "SOCKET: " << sock.getSocket() << std::endl; // VERIFICATION uniquement
		// std::cout << "IF 0, VALID: " << sock.getValidity() << std::endl; // VERIFICATION uniquement


		//------------------------- BIND() -------------------------//
		Server	srv;
		if (argc < 3)
			throw (std::exception());

		srv.setPort(atoi(argv[1]));
		std::cout << "PORT: " << srv.getPort() << std::endl;
		
		srv.createSocket();
		// srv.createSocket();
		// close(srv.getSocket());
		// srv.createSocket();

		srv.allSockets();

		// -------- tests

		struct sockaddr_in	addr;

		bzero(&addr, sizeof(addr));
		int fd_client;
		unsigned int len;
		char buf[250];
		int ret;

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

			}
			// else
			// {
			// 	std::cout << "dommage" << std::endl;
			// }
		}

		// ------ fin tests


		//------------------------- LISTEN() -------------------------//

		//------------------------- POLL() / SELECT() -------------------------//

		//------------------------- ACCEPT() -------------------------//

		//------------------------- SEND() -------------------------//

		//------------------------- CLOSE() -------------------------//


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
