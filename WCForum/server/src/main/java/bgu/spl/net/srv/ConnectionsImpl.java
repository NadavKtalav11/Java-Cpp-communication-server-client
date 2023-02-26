package bgu.spl.net.srv;

//import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionsImpl <T> implements Connections<T>{


    private ConcurrentHashMap <Integer,ConnectionHandler<T>> clientCH; // each Id for client connectionH
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<String>> channels ; // for each game, witch user are subscribed
    private ConcurrentHashMap<String , Integer> username_connectId; // which user are currently logged in
    private ConcurrentHashMap <String,String> userName_userPassword; // user name and it's password
    private ConcurrentHashMap <Integer,String> connectionId_userName; //
    int messageId ;


    public ConnectionsImpl() {
        clientCH = new ConcurrentHashMap<Integer,ConnectionHandler<T>>() ;
        channels = new ConcurrentHashMap<String, ConcurrentLinkedQueue<String>>();
        username_connectId = new ConcurrentHashMap<String ,Integer> ();
        connectionId_userName = new ConcurrentHashMap<Integer,String>();
        userName_userPassword = new ConcurrentHashMap<String, String>();
        messageId =1;
    }

     public synchronized int getMessageId()
    {
        int id = messageId;
        messageId++;
        return id;
    }


    @Override
    public boolean send(int connectionId, T msg) {
    try {
        clientCH.get(connectionId).send(msg);
    }
    catch(Exception e){
        return false;
    }
    return true;
    }

    @Override
    public void send(String channel, T msg) {
        ConcurrentLinkedQueue <String> curr =  channels.get(channel);
         for (String username: curr){
            int currConnectionID = username_connectId.get(username);
            send(currConnectionID , msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {

        String userName = connectionId_userName.get(connectionId);
        removeClientFromAllisTopic(userName);
        username_connectId.remove(userName);
        clientCH.remove(connectionId);

    }

    public void addCH(int id, ConnectionHandler<T> ch){
        clientCH.put(id, ch);
    }


    public void connect(int connectionId , String username, String password ) {

        connectionId_userName.put(connectionId,username);
        username_connectId.put(username , connectionId);
        if(!userName_userPassword.containsKey(username)){
        userName_userPassword.put(username,password);}
    }



    @Override
    public void Subscribe(int connectionid , int id , String topic) {
        ConcurrentLinkedQueue<String> currChannel = channels.get(topic);
        String currUser = connectionId_userName.get(connectionid);
        if (currChannel==null){
            currChannel = new ConcurrentLinkedQueue<>();
            currChannel.add(currUser);
            channels.put(topic , currChannel);
        }
        else {
            currChannel.add(currUser);
        }
    }
    public void unSubscribe(int connectionId , String topic) {
        String currUser = connectionId_userName.get(connectionId);
        ConcurrentLinkedQueue<String> currChannel = channels.get(topic);
        currChannel.remove(currUser);
        if (currChannel.size()==0){
            channels.remove(topic);
        }
    }
    public boolean checkNameIsAvailable(String name){
        if (userName_userPassword.containsKey(name)){
            return false;
        }
        return true;
    }

    public boolean checkPasswordByName(String name , String password){
        if (userName_userPassword.get(name).equals(password)){
            return true;
        }
        return false;
    }

    public boolean checkIfUserLoggedIn(String name){
        if(username_connectId.containsKey(name)){
            return true;
        }
        return false;
    }

    public boolean checkIfUserSubscribedToTopic(String topic, int connectionID){
        String currUser = connectionId_userName.get(connectionID);
        if(channels.get(topic)!=null){
            ConcurrentLinkedQueue<String> currChannel = channels.get(topic);
            if(currChannel.contains(currUser)){
                return true;
            }
        }
        return false;
    }

    public void removeClientFromAllisTopic(String username){
        for (ConcurrentLinkedQueue<String> q: channels.values()) {
            if(q.contains(username)){
                q.remove(username);
        }
        }
    }
}
