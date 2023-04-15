#include "Socket.hpp" // NADIA

// mode d'emploi : ./ircserv <port> <password>
int	main(int argc, char *argv[])
{
	// NADIA
	try
	{
		//gestion d'erreur ici a ajouter lors du parsing
		(void)argc;
		(void)argv;


		// int	tempura;
		// struct addrinfo addr;
		

		//------------------------- SOCKET() & SETSOCKOPT() -------------------------//
		// int	error = errno;
		Socket	sock;
		std::cout << sock.getSocket() << std::endl; // VERIFICATION uniquement
		std::cout << sock.getValidity() << std::endl; // VERIFICATION uniquement






		//------------------------- BIND() -------------------------//

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
