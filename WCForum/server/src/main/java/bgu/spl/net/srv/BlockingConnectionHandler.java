package bgu.spl.net.srv;

//import bgu.spl.net.api.MessageEncoderDecoder;
//import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.STOMPEncoderDecoder;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocol<T> protocol;
    private final STOMPEncoderDecoder encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private Connections<T> conn;
    int id ;


    public BlockingConnectionHandler(Socket sock, STOMPEncoderDecoder reader, StompMessagingProtocol<T> protocol ,Connections<T> conn , int id) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.conn = conn;
        this.id = id;
        conn.addCH(id , this);

    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;
            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());
            protocol.start(id, conn);

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                String nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    String response = protocol.process(nextMessage);
                    if (response != null) {
                        out.write(encdec.encode(response));
                        out.flush();
                    }
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        //IMPLEMENT IF NEEDED
        try {
            out.write(encdec.encode((String)msg));
            out.flush();
        }
        catch (IOException ioE){
            System.out.println(ioE);
        }




    }
}
