// #include "Socket.hpp" // NADIA
#include "Server.hpp" // NADIA

// mode d'emploi : ./ircserv <port> <password>
int	main(int argc, char *argv[])
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NADIA
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		// struct sockaddr_in	addr;
		Server				srv;


		if (argc < 3)
			throw (std::exception()); // usage ./ircserv <port> <password> a afficher

		srv.setPort(atoi(argv[1]));
		srv.createSocket();

		srv.allSockets(); // DEBUG ONLY

		// bzero(&srv._addr, sizeof(srv._addr));
		// while (true)
		// {
			srv.pollConnection();
		// }

		// -------- tests

		// struct sockaddr_in	addr;

		// bzero(&addr, sizeof(addr));
		// int fd_client;
		// unsigned int len;
		// char buf[250];
		// int ret;
		// char basic[] = "je suis arrive";

		// while (true)
		// {
		// 	len = sizeof(addr);
		// 	// fd_client = accept(srv.getSocket(), NULL, NULL); // ne renvoie pas l'adresse
		// 	fd_client = accept(srv.getSocket(), (struct sockaddr *)&addr, &len);
		// 	if (fd_client > 0)
		// 	{
		// 		std::cout << "client accepte !" << std::endl;
		// 		std::cout << inet_ntoa(addr.sin_addr) << std::endl; //affichage de l'adresse en decimal du client
		// 		ret = recv(fd_client, &buf, sizeof(buf), 0);
		// 		write(1, buf, ret);
		// 		send(fd_client, basic, sizeof(basic), 0);

		// 	}
		// 	// else
		// 	// {
		// 	// 	std::cout << "dommage" << std::endl;
		// 	// }
		// }

	// 	// SOURCE IBM TO TEST ONLY
	// 	struct pollfd fds[200];
	// 	int	nfds = 1;
	// 	int	current_size = 0;
	// 	int	i;
	// 	int	j;
	// 	// int	timeout;
	// 	int	rc;
	// 	int	end_server = 0;
	// 	int	new_sd = -1;
	// 	int	close_conn;
	// 	char	buffer[80];
	// 	int	len;
	// 	int	compress_array = 0;


	// 	/*************************************************************/
	// 	/* Initialize the pollfd structure                           */
	// 	/*************************************************************/
	// 	memset(fds, 0 , sizeof(fds));

	// 	/*************************************************************/
	// 	/* Set up the initial listening socket                        */
	// 	/*************************************************************/
	// 	fds[0].fd = srv.getSocket();
	// 	fds[0].events = POLLIN;
	// 	/*************************************************************/
	// 	/* Initialize the timeout to 3 minutes. If no                */
	// 	/* activity after 3 minutes this program will end.           */
	// 	/* seulement quand 3 * 60 * 1000 c'est 3 minutes             */
	// 	/* timeout value is based on milliseconds.                   */
	// 	/*************************************************************/
	// 	// timeout = (30 * 1000);

	// 	/*************************************************************/
	// 	/* Loop waiting for incoming connects or for incoming data   */
	// 	/* on any of the connected sockets.                          */
	// 	/*************************************************************/
	// 	do
	// 	{
	// 		/***********************************************************/
	// 		/* Call poll() and wait 3 minutes for it to complete.      */
	// 		/***********************************************************/
	// 		printf("Waiting on poll()...\n");
	// 		rc = poll(fds, nfds, TIMEOUT_NO);

	// 		/***********************************************************/
	// 		/* Check to see if the poll call failed.                   */
	// 		/***********************************************************/
	// 		// if (rc < 0) -- original
	// 		if (rc <= 0)
	// 		{
	// 			perror("  poll() failed");
	// 			break;
	// 		}

	// 		/***********************************************************/
	// 		/* Check to see if the 3 minute time out expired.          */
	// 		/***********************************************************/
	// 		// if (rc == 0)
	// 		// {
	// 		// 	printf("  poll() timed out.  End program.\n");
	// 		// 	break;
	// 		// }


	// 		/***********************************************************/
	// 		/* One or more descriptors are readable.  Need to          */
	// 		/* determine which ones they are.                          */
	// 		/***********************************************************/
	// 		current_size = nfds;
	// 		for (i = 0; i < current_size; i++)
	// 		{
	// 			/*********************************************************/
	// 			/* Loop through to find the descriptors that returned    */
	// 			/* POLLIN and determine whether it's the listening       */
	// 			/* or the active connection.                             */
	// 			/*********************************************************/
	// 			if(fds[i].revents == 0)
	// 				continue;

	// 			/*********************************************************/
	// 			/* If revents is not POLLIN, it's an unexpected result,  */
	// 			/* log and end the server.                               */
	// 			/*********************************************************/
	// 			if(fds[i].revents != POLLIN)
	// 			{
	// 				printf("  Error! revents = %d\n", fds[i].revents);
	// 				end_server = 1;
	// 				break;
	// 			}

	// 			std::cout << "if " << fds[i].fd << " == " << srv.getSocket() << std::endl;

	// 			if (fds[i].fd == srv.getSocket())
	// 			{
	// 				// nouvel utilisateur arrive ici

	// 				/*******************************************************/
	// 				/* Listening descriptor is readable.                   */
	// 				/*******************************************************/
	// 				printf("(fds[i].fd == _socket)  Listening socket is readable\n");

	// 				/*******************************************************/
	// 				/* Accept all incoming connections that are            */
	// 				/* queued up on the listening socket before we         */
	// 				/* loop back and call poll again.                      */
	// 				/*******************************************************/
	// 				do
	// 				{
	// 					/*****************************************************/
	// 					/* Accept each incoming connection. If               */
	// 					/* accept fails with EWOULDBLOCK, then we            */
	// 					/* have accepted all of them. Any other              */
	// 					/* failure on accept will cause us to end the        */
	// 					/* server.                                           */
	// 					/*****************************************************/
	// 					new_sd = accept(srv.getSocket(), NULL, NULL);
	// 					if (new_sd < 0)
	// 					{
	// 						if (errno != EWOULDBLOCK)
	// 						{
	// 							perror("  accept() failed");
	// 							end_server = 1;
	// 						}
	// 						break;
	// 					}

	// 					/*****************************************************/
	// 					/* Add the new incoming connection to the            */
	// 					/* pollfd structure                                  */
	// 					/*****************************************************/
	// 					printf("(accept OK)  New incoming connection - %d\n", nfds);
	// 					std::cout << "new fd: " << new_sd << std::endl;
	// 					fds[nfds].fd = new_sd;
	// 					fds[nfds].events = POLLIN;
	// 					nfds++;

	// 				/*****************************************************/
	// 				/* Loop back up and accept another incoming          */
	// 				/* connection                                        */
	// 				/*****************************************************/
	// 				} while (new_sd != -1);
	// 			}

	// 			/*********************************************************/
	// 			/* This is not the listening socket, therefore an        */
	// 			/* existing connection must be readable                  */
	// 			/*********************************************************/

	// 			else
	// 			{
	// 				printf("  Descriptor %d is readable\n", fds[i].fd);
	// 				close_conn = 0;
	// 				/*******************************************************/
	// 				/* Receive all incoming data on this socket            */
	// 				/* before we loop back and call poll again.            */
	// 				/*******************************************************/

	// 				do
	// 				{
	// 					/*****************************************************/
	// 					/* Receive data on this connection until the         */
	// 					/* recv fails with EWOULDBLOCK. If any other         */
	// 					/* failure occurs, we will close the                 */
	// 					/* connection.                                       */
	// 					/*****************************************************/
	// 					rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
	// 					if (rc < 0)
	// 					{
	// 						if (errno != EWOULDBLOCK)
	// 						{
	// 							perror("  recv() failed");
	// 							close_conn = 1;
	// 						}
	// 						break;
	// 					}

	// 					/*****************************************************/
	// 					/* Check to see if the connection has been           */
	// 					/* closed by the client                              */
	// 					/*****************************************************/
	// 					if (rc == 0)
	// 					{
	// 						printf("  Connection closed\n");
	// 						close_conn = 1;
	// 						break;
	// 					}

	// 					/*****************************************************/
	// 					/* Data was received                                 */
	// 					/*****************************************************/
	// 					len = rc;
	// 					printf("  %d bytes received\n", len);

	// 					/*****************************************************/
	// 					/* Echo the data back to the client                  */
	// 					/*****************************************************/
	// 					for (int j = 0; j < current_size; j++)
	// 					{
	// 						if (j != i)
	// 							rc = send(fds[j].fd, buffer, len, 0);
	// 					}
	// 					if (rc < 0)
	// 					{
	// 						perror("  send() failed");
	// 						close_conn = 1;
	// 						break;
	// 					}

	// 				} while(1);

	// 					/*******************************************************/
	// 					/* If the close_conn flag was turned on, we need       */
	// 					/* to clean up this active connection. This            */
	// 					/* clean up process includes removing the              */
	// 					/* descriptor.                                         */
	// 					/*******************************************************/
	// 					if (close_conn)
	// 					{
	// 						close(fds[i].fd);
	// 						fds[i].fd = -1;
	// 						compress_array = 1;
	// 					}
	// 				}  /* End of existing connection is readable             */
	// 			} /* End of loop through pollable descriptors              */

	// 			/***********************************************************/
	// 			/* If the compress_array flag was turned on, we need       */
	// 			/* to squeeze together the array and decrement the number  */
	// 			/* of file descriptors. We do not need to move back the    */
	// 			/* events and revents fields because the events will always*/
	// 			/* be POLLIN in this case, and revents is output.          */
	// 			/***********************************************************/
	// 			if (compress_array)
	// 			{
	// 				compress_array = 0;
	// 				for (i = 0; i < nfds; i++)
	// 				{
	// 					if (fds[i].fd == -1)
	// 					{
	// 						for(j = i; j < nfds; j++)
	// 						{
	// 							fds[j].fd = fds[j+1].fd;
	// 						}
	// 						i--;
	// 						nfds--;
	// 					}
	// 				}
	// 			}

	// 	} while (end_server == 0); /* End of serving running.    */

	// 	/*************************************************************/
	// 	/* Clean up all of the sockets that are open                 */
	// 	/*************************************************************/
	// 	for (i = 0; i < nfds; i++)
	// 	{
	// 		if(fds[i].fd >= 0)
	// 			close(fds[i].fd);
	// 	}


	// 	// ------ fin tests

		return (0);
	}
	catch(const std::exception& e)
	{
		// std::cerr << errno << '\n';
		std::cerr << e.what() << '\n';
	}

	// std::cout	<< error << std::endl;

	//*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CLAIRE
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//* CODE DE CLAIRE ICI

	//*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////







	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VERENA
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* CODE DE VERENA ICI

	*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	// return (0);
}
