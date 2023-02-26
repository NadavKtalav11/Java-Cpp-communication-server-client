package bgu.spl.net.impl.stomp;

//import java.awt.*;

public class StompFrame {
    private String stompCommand;
    private String [] headerName;
    private String frameBody;
    //private boolean receipt;

    public StompFrame (String message){
        String lines[] = message.split("\\r?\\n");
        if (lines.length>0) {
            this.stompCommand = lines[0];
        }
        if (stompCommand.equals("DISCONNECT")){
            headerName = new String[1];
            headerName[0]= lines[1];
            frameBody = "";
        }
        if (stompCommand.equals("CONNECT")){
            headerName = new String[4];
            for(int i = 1 ; i<5 ; i++){
                headerName[i-1]= lines[i];
            }
            frameBody = "";
        }
        if (stompCommand.equals("SUBSCRIBE")){
            headerName = new String[3];
            for(int i = 0 ; i<headerName.length ; i++){
                headerName[i]= lines[i+1];
            }
            frameBody = "";
        }
        if (stompCommand.equals("UNSUBSCRIBE")){
            headerName = new String[2];
            for(int i = 0 ; i<headerName.length ; i++){
                headerName[i]= lines[i+1];
            }
            frameBody = "";
        }
        if (stompCommand.equals("SEND")){
            headerName = new String[1];
            for(int i = 0 ; i<headerName.length ; i++){
                headerName[i]= lines[i+1];
            }
            for (int i=0 ; i< lines.length-2 ; i++){
                frameBody = frameBody + "/n"  + lines[i+2];
            }
        }

    }

    public String ifReceiptExists(){
        String receipt="";
        for(int i=0; i< headerName.length;i++){
            if (headerName[i].length()>8){
                if(headerName[i].substring(0,8).equals("receipt:")){
                    receipt =headerName[i].substring(9);
                    return receipt;
                }
            }
        }
        return receipt;
    }


    public String getStompCommand(){
        return stompCommand;
    }
    public String[] getHeaderName(){
        return headerName;
    }
    public String getFrameBody(){
        return frameBody;
    }
}
