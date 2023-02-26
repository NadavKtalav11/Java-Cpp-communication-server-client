package bgu.spl.net.srv;

//import java.io.IOException;

public interface Connections <T>{

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    void connect(int connectionId, String userName, String password);

    public void addCH(int id, ConnectionHandler<T> ch);


    public void Subscribe(int connectionId , int id, String topic);

    public void unSubscribe(int ClientId , String topic);

    public boolean checkNameIsAvailable(String name);

    public boolean checkPasswordByName(String name , String password);

    public boolean checkIfUserLoggedIn(String name);
    public boolean checkIfUserSubscribedToTopic(String topic, int connectionID);

    public void removeClientFromAllisTopic(String username);

    public int getMessageId();


}
