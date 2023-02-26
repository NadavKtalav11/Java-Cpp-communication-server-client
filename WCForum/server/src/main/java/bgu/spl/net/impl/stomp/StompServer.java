package bgu.spl.net.impl.stomp;

//import bgu.spl.net.impl.echo.EchoProtocol;
//import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
//import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        
        // just for debbugging
        if (args.length!=2) {
            Server.reactor(4, 
            7777, 
            new ConnectionsImpl<String>(),
            () -> new StompProtocol(), 
            STOMPEncoderDecoder::new).serve();
        } 
        else {
            if (args[1].equals("tpc")){
        Server.threadPerClient(
                new ConnectionsImpl<String>(),
                7777, //port
                () -> new StompProtocol(), //protocol factory
                STOMPEncoderDecoder::new //message encoder decoder factory
        ).serve();
            }
            else if (args[1].equals("reactor")){
                Server.reactor(4, 7777, new ConnectionsImpl<String>(),() -> new StompProtocol(), STOMPEncoderDecoder::new).serve();
            }
            else {
                System.out.print("server needs a leagel Port and Type");
            }
        }
    }
}
