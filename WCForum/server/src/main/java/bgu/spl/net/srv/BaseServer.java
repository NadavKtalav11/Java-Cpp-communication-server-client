package bgu.spl.net.srv;


import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.STOMPEncoderDecoder;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMessagingProtocol<T>> protocolFactory;
    private final Supplier<STOMPEncoderDecoder> encdecFactory;
    private ServerSocket sock;
    private Connections<T> con ;
    private int idCounter;
   // public static int messageId=1;

    public BaseServer(
            int port,
            Supplier<StompMessagingProtocol<T>> protocolFactory,
            Supplier<STOMPEncoderDecoder> encdecFactory , Connections<T> _con) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;
        this.con = _con;
        idCounter = 0;

    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close


            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<T>(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get(),
                        con,
                        idCounter++);

                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}


