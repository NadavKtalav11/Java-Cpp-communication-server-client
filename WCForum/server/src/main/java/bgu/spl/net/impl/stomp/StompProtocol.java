package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
//import bgu.spl.net.srv.BaseServer;
import bgu.spl.net.srv.Connections;
//import bgu.spl.net.srv.NonBlockingConnectionHandler;

//import java.time.LocalDateTime;
//import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
//import java.util.concurrent.ConcurrentLinkedQueue;

public class StompProtocol implements StompMessagingProtocol <String> {

    private boolean shouldTerminate = false;
    private int connectionId ;
    private Connections<String> conn;

    // subscription id and game name
    private ConcurrentHashMap<Integer , String> SubscribeIDs = new ConcurrentHashMap<Integer , String>();
    private ConcurrentHashMap<String , Integer> topic_to_subId = new ConcurrentHashMap<String , Integer>();



    @Override
    public void start(int connectionId, Connections <String> connections) {

        this.connectionId = connectionId ;
        conn = connections;

    }


    
    public String process(String message) {
        System.out.println("start process" + message);
        StompFrame currFrame = new StompFrame(message);
        if (currFrame.getStompCommand().equals("SEND")){
            String topic= currFrame.getHeaderName()[0];
            topic =topic.substring(12);
            String receiptExist = currFrame.ifReceiptExists();

             if (!conn.checkIfUserSubscribedToTopic(topic , connectionId)){
                conn.disconnect(connectionId);
                return ErrorFrame(message ,"Report Problem" , "you can't report on a game" +
                        "you aren't subscribed" , receiptExist  );
            }
            //String header = "";
            //for (int j=0 ; j<currFrame.getHeaderName().length;j++){
            //     header = header + currFrame.getHeaderName()[j]+'\n';
            //}
            int subId = topic_to_subId.get(topic);
            String new_message = message.substring(8);
            conn.send(topic ,"MESSAGE" +"\n"
                        + "subscription:" + subId + "\n"
                        + "message-id:" + conn.getMessageId() +"\n"
                        + new_message);

        }
        else if (currFrame.getStompCommand().equals("SUBSCRIBE")){
            String topic= currFrame.getHeaderName()[0];
            topic =topic.substring(12);
            String subId = currFrame.getHeaderName()[1];
            subId = subId.substring(3);
            int IntId = Integer.parseInt(subId);
            String receiptExist = currFrame.ifReceiptExists();

            if (conn.checkIfUserSubscribedToTopic(topic , connectionId)) {
                conn.disconnect(connectionId);
                return ErrorFrame(message ,"subscription Problem" , "this user is already subscribed to this game" , receiptExist );
            }
                conn.Subscribe(connectionId , IntId , topic);
                 String recieptId = currFrame.getHeaderName()[2];
                 recieptId = recieptId.substring(8);
                 //int intRecieptId = Integer.parseInt(recieptId);
                 SubscribeIDs.put(IntId,topic);
                 topic_to_subId.put(topic, IntId);
                 String frameToReturn = "RECEIPT" + "\n" + "receipt-id:" + recieptId + "\n";
                 return frameToReturn;
        }
        else if (currFrame.getStompCommand().equals("UNSUBSCRIBE")) {
            String id = currFrame.getHeaderName()[0];
            id = id.substring(3);
            int intId = Integer.parseInt(id);
            String topic = SubscribeIDs.get(intId);
            String receiptExist = currFrame.ifReceiptExists();
            if (intId==-1){
                conn.disconnect(connectionId);
                return ErrorFrame(message, "subscribe Problem", "this user hasn't Subscribe yet", receiptExist);
            }
             if (!conn.checkIfUserSubscribedToTopic(topic, connectionId)) {
                conn.disconnect(connectionId);
                return ErrorFrame(message, "subscribe Problem", "this user hasn't Subscribe yet", receiptExist);
            }
            else {
                String recieptid = currFrame.getHeaderName()[1];
                recieptid = recieptid.substring(8);
                conn.unSubscribe(connectionId ,topic);
                 SubscribeIDs.remove(connectionId);
                 topic_to_subId.remove(topic);
                 String frameToReturn = "RECEIPT" + "\n" + "receipt-id:" + recieptid + "\n";
                 return frameToReturn;
             }
        }
        if (currFrame.getStompCommand().equals("CONNECT")){
            String receiptExist = currFrame.ifReceiptExists();

            String[] headerName = currFrame.getHeaderName();
            String version = headerName[0];
            if (!version.equals("accept-version:1.2")){
                //return new errorMessage
                //conn.disconnect(connectionId);
                return ErrorFrame(message, "version Problem", "the version is not math to the server", receiptExist);
            }
            String host = headerName[1];
            if (!host.equals("host:stomp.cs.bgu.ac.il")){
                //return new errorMessage
                //conn.disconnect(connectionId);
                return ErrorFrame(message, "IP Problem", "the address is not match the Server", receiptExist);
            }
            String loginName = headerName[2].substring(6);
            if (!conn.checkNameIsAvailable(loginName)){
                if(conn.checkIfUserLoggedIn(loginName)){
                    conn.disconnect(connectionId);
                    return ErrorFrame(message, "User already loggedIn", "user already loggedIn to the server", receiptExist);
            }
                if(!conn.checkPasswordByName(loginName,headerName[3].substring(9))){
                //return new errorMessage
                //conn.disconnect(connectionId);
                return ErrorFrame(message, "Wrong password", "the userName and the password don't match", receiptExist);}
            }
            conn.connect(connectionId, loginName, headerName[3].substring(9));
            String successfulConnection = "CONNECTED" + "\n" +"version:1.2" +"\n";
            return successfulConnection;
        }
        if (currFrame.getStompCommand().equals("DISCONNECT")){
            //String receiptExist = currFrame.ifReceiptExists();
            conn.disconnect(connectionId);
            String successfulDisConnection = "RECEIPT" + "\n" +"receipt-id:"+currFrame.getHeaderName()[0].substring(8)+ "\n";
            shouldTerminate = true;
            return successfulDisConnection;
        }

        return null;
    }


    public String ErrorFrame(String message ,String errorMassage , String errorDescription , String id){
        String errorFrame = "ERROR" + "\n" + "receipt-id:"+id + "\n" + "message:" + errorMassage +"\n";
        errorFrame = errorFrame +"\n" + "The message:" + "\n" + "-----" + "\n" + message;
        errorFrame= errorFrame+ "\n" + "-----" + "\n" + errorDescription;
        return errorFrame;
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public int getId (){
        return connectionId;
    }

    public Connections<String> getConnections (){
        return conn;
    }
}
