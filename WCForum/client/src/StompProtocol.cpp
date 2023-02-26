using namespace std;
#include <bits/stdc++.h>
#include <string>
#include <vector>
#include "../include/event.h"
#include "../include/StompProtocol.h"
#include "../include/Game_data.h"




StompProtocol::StompProtocol(): game_Data() 
{ 
    
}

StompProtocol::~StompProtocol()
{

}
    
std::vector<std::string> StompProtocol::processOut(std::string massage){

    vector <string> framesToReturn;
    if (massage.length()==0){
        std::cout << "illegal command , please try again" << std::endl;
        return framesToReturn;
    }
    string frame;
    vector <string> seperated;

    stringstream ss(massage);
    std::string word;
    //game_Data;
    
    int i=0;
    while (ss>>word){
        seperated.push_back(word);
        i=i+1;
    }
    int numOfWords= seperated.size();

    if (seperated.at(0).compare("login")==0){
        // login
        frame = frame + "CONNECT" + '\n' ;
        frame = frame + "accept-version:1.2" + '\n';
        frame = frame + "host:stomp.cs.bgu.ac.il" + '\n';
        frame = frame + "login:" + seperated.at(2) + '\n';
        frame = frame + "passcode:" + seperated[3] + '\n';
        framesToReturn.push_back(frame);
        game_Data.setActiveUser(seperated[2]);
        std::cout<< "try to connect"<< std::endl;              
    }
    else if (seperated[0].compare("join")==0){
        //join
        if (numOfWords!=2){
            std::cout << "illegal command , please try again\n" << std::endl;
            return framesToReturn;
        }
        frame = frame + "SUBSCRIBE" + '\n' ;
        frame = frame + "destination:/" + seperated[1]+ '\n';
        string id1 =  game_Data.getNewSubscriptionId();
        //int id2 = std::stoi(id1);
        frame = frame + "id:"  + id1 + '\n'; 
        string recieptID = game_Data.getNewreceiptId();
        frame =  frame + "receipt:" + recieptID  + '\n'; 
        framesToReturn.push_back(frame);
        game_Data.topics_subscriptionId[seperated[1]]=id1;
        game_Data.reciept_command[recieptID]= "SUBSCRIBE"; 
        game_Data.reciept_game[recieptID]= seperated[1]; 
    }

    else if (seperated[0].compare("exit")==0){
        //exit
        if (numOfWords!= 2){
            std::cout << "illegal command , please try again\n" << std::endl;
            return framesToReturn;
        }
        frame = frame + "UNSUBSCRIBE" + '\n' ;
        string id =game_Data.topics_subscriptionId[seperated[1]];
        if (id.length()==0){
            id = "-1";
        }
        frame = frame + "id:" + id + '\n';
        string recieptId = game_Data.getNewreceiptId();
        frame = frame + "receipt:" + recieptId + '\n';
        framesToReturn.push_back(frame);
        game_Data.topics_subscriptionId.erase(seperated[1]);
        game_Data.reciept_command[recieptId]="UNSUBSCRIBE";
        game_Data.reciept_game[recieptId]= seperated[1];
    }
    else if (seperated[0].compare("report")==0){
        // report
        if (numOfWords!= 2)
        {
            std::cout << "illegal command , please try again\n" << std::endl;
            return framesToReturn;
        }
        string path = "../../client/data/" +seperated[1];
        names_and_events _names_and_events = parseEventsFile(path);
        int numOfEvents = _names_and_events.events.size();
        for(int i=0; i<numOfEvents;i++){
        frame = "";
        frame = frame + "SEND" + '\n' ;
        frame = frame + "destination:/" +_names_and_events.team_a_name+ "_"+ _names_and_events.team_b_name + '\n';
        //frame = frame + '\n';
        frame = frame + "user:" + game_Data.getActiveUser() + '\n' ; 
        frame = frame + "team a:" + _names_and_events.events[i].get_team_a_name() + '\n';
        frame = frame + "team b:" + _names_and_events.events[i].get_team_b_name() + '\n';
        frame = frame + "event name:" + _names_and_events.events[i].get_name() + '\n';
        std::string time = std::to_string(_names_and_events.events[i].get_time());
        frame = frame + "time:" + time + '\n';
        frame = frame + "general game updates:" + '\n';
        for(auto it = _names_and_events.events[i].get_game_updates().cbegin(); it != _names_and_events.events[i].get_game_updates().cend(); ++it)
        {
           frame= frame +  it->first +":"+ it->second + '\n';
        }
       
        frame = frame + "team a updates:" + '\n';
        for(auto it = _names_and_events.events[i].get_team_a_updates().cbegin(); it != _names_and_events.events[i].get_team_a_updates().cend(); ++it)
        {
           frame= frame +  it->first +":"+ it->second + '\n';
        }
        frame = frame + "team b updates:" + '\n';
        for(auto it = _names_and_events.events[i].get_team_b_updates().cbegin(); it != _names_and_events.events[i].get_team_b_updates().cend(); ++it)
        {
           frame= frame +  it->first +':'+ it->second + '\n';
        }
        frame = frame + "description:" + '\n';
        frame = frame + _names_and_events.events[i].get_discription();
        framesToReturn.push_back(frame);
    }
    }

    else  if (seperated[0].compare("summary")==0){
        // summary
        if (numOfWords!=4){
            std::cout << "illegal command , please try again\n" << std::endl;
            return framesToReturn;
        }
        int pos = seperated[1].find_first_of('_');
        std::string teamB = seperated[1].substr(pos+1),
        teamA= seperated[1].substr(0, pos);

        /// sort the events of the user for the game acording to chronological time
        std::map<std::pair<std::string, std::string>, std::vector<Event> > map1 = game_Data.userAndTopicForEvent;
        std::pair<std:: string, std::string> pair1 = std::make_pair(seperated[1],seperated[2]);
        std::vector<Event> events_user_for_topic = map1[pair1];
        std::sort(events_user_for_topic.begin(), events_user_for_topic.end(), [](Event _event1, Event _event2){    
        std::map<std::string, std::string> map1 = _event1.get_game_updates();
        std::map<std::string, std::string> map2 = _event2.get_game_updates();
        if(map1["before halftime"]=="true" && map1["before halftime"]=="false" ){
            return false;
        }
        else if(map1["before halftime"]=="false" && map1["before halftime"]=="true"){
            return true;
        }
        else{
            int event1= _event1.get_time();
            int event2 = _event2.get_time();
            return (event1 < event2);
        }});
        
        frame = teamA + " vs " + teamB +'\n';
        frame = frame + "Game stats:" + '\n';
        frame = frame + "General stats:" + "\n";
        frame = frame + "active:false" + '\n';
        frame = frame + "before halftime:false" + '\n';
        frame = frame + teamA + " stats:" + '\n';
        frame = frame + "goals:"  + game_Data.goalAndPossession[pair1]["Agoals"] + '\n';
        frame = frame + "possession:" + game_Data.goalAndPossession[pair1]["Apossession"] + '\n'; 
        frame = frame + teamB + " stats:" + '\n';
        frame = frame + "goals:"  + game_Data.goalAndPossession[pair1]["Bgoals"] + '\n';
        frame = frame + "possession:" + game_Data.goalAndPossession[pair1]["Bpossession"] + '\n'; 
        frame = frame + "Game event reports:" +'\n';
        int numOftopics = events_user_for_topic.size();
        for(int i=0;i<numOftopics;i++){
            std::string time = std::to_string(events_user_for_topic[i].get_time());
            frame = frame + time + "-" + events_user_for_topic[i].get_name() + '\n';
            frame = frame + events_user_for_topic[i].get_discription()+ '\n';
        }
        writeToFile(frame , seperated[3]);

        
    }
        
    else if (seperated[0].compare("logout")==0){
        // report
        if (numOfWords!=1){
            std::cout << "illegal command , please try again\n" << std::endl;
            return framesToReturn;
        }
        frame = frame + "DISCONNECT" + '\n';
        string recieptId = game_Data.getNewreceiptId();
        frame = frame + "receipt:" + recieptId;
        framesToReturn.push_back(frame);
        game_Data.reciept_command[recieptId]= "DISCONNECT";
    } 
    return framesToReturn;
} 


bool StompProtocol::processIn(std::string &massage){
    string copy(massage.begin(), massage.end()); 
    vector <std::string> seperated ;
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    while ((pos = copy.find(delimiter)) != std::string::npos) {
        token = copy.substr(0, pos);
        seperated.push_back(token);
        copy.erase(0, pos + delimiter.length());
    } 
    seperated.push_back(copy);          
    
    if (seperated[0].compare("CONNECTED")==0){
        std::cout << "connected suuccessfuly" << std::endl;

    }
    else if (seperated[0].compare("MESSAGE")==0){
        //join
        //std::cout<<massage<<endl;
        
        int idx = seperated[3].find(':') ;
        string destination= seperated[3].substr(idx+2);
        idx = seperated[4].find(':') ;
        string userThatSend= seperated[4].substr(idx+1);
        idx = seperated[5].find(':') ;
        string teamA= seperated[5].substr(idx+1);
        idx = seperated[6].find(':') ;
        string teamB= seperated[6].substr(idx+1);
        idx = seperated[7].find(':') ;
        string eventName= seperated[7].substr(idx+1);
        idx = seperated[8].find(':') ;
        string time= seperated[8].substr(idx+1);
        int row = 10;
        std::map<std::string , std::string> gameUpdates;
        while (seperated[row].compare("team a updates:")!= 0){
            idx = seperated[row].find(':') ;
            string currUpdatekey= seperated[row].substr(0,idx);
            string currUpdateVal= seperated[row].substr(idx+1);
            gameUpdates[currUpdatekey]= currUpdateVal;
            row= row+1;
        }
        row = row+1;
        std::map<std::string , std::string> teamAUpdates;
        while (seperated[row].compare("team b updates:")!= 0){
            idx = seperated[row].find(':') ;
            string currUpdatekey= seperated[row].substr(0,idx);
            string currUpdateVal= seperated[row].substr(idx+1);
            teamAUpdates["A"+ currUpdatekey]= currUpdateVal;
            row= row+1;
        }
        row = row+1;
        std::map<std::string , std::string> teamBUpdates;
        while (seperated[row].compare("description:")!= 0){
            idx = seperated[row].find(':') ;
            string currUpdatekey= seperated[row].substr(0,idx);
            string currUpdateVal= seperated[row].substr(idx+1);
            teamBUpdates["B" + currUpdatekey]= currUpdateVal;
            row= row+1;
        }
        std::string desc= seperated[row+1];
        //std:string desc;
        //for (int k=row+1 ; k<seperated.size(); row++){
        //    desc = desc + seperated[row] + '\n';
        //}
        int intTime = std::stoi(time);
        Event newEvent(teamA ,teamB ,eventName , intTime, gameUpdates, teamAUpdates, teamBUpdates , desc);
        std::pair<std::string,std::string> nameAndGame = std::make_pair( destination ,userThatSend);
        map<std::string,std::string>:: iterator itA;
        for (itA=teamAUpdates.begin();itA!=teamAUpdates.end(); itA++){
            game_Data.goalAndPossession[nameAndGame][itA->first]= itA->second;
        }
        map<std::string,std::string>:: iterator itb;
        for (itb= teamBUpdates.begin();itb!=teamBUpdates.end(); itb++){
            game_Data.goalAndPossession[nameAndGame][itb->first]= itb->second;
        }
        game_Data.userAndTopicForEvent[nameAndGame].push_back(newEvent);
        std::cout<<"you got this message "  <<endl;
        std::cout<< massage <<endl;
    }
    else if (seperated[0].compare("RECEIPT")==0){
        
        std::string delimiter = ":";
        int pos =0;
        std::string token;
        pos = seperated[1].find(delimiter) ;
        string recieptId = seperated[1].substr(pos+1);
        string command = game_Data.reciept_command[recieptId];
        if (command.compare("UNSUBSCRIBE")==0 ){
            std::cout<< "Exited channel "+ game_Data.reciept_game[recieptId] <<std::endl;
            game_Data.reciept_game.erase(recieptId);
        }
        else if (command.compare("SUBSCRIBE")==0 ){
            std::cout<< "Joined channel "+ game_Data.reciept_game[recieptId] <<std::endl;
            game_Data.reciept_game.erase(recieptId);
        }
        else if (command.compare("DISCONNECT")==0 ){
            //std::cout<< "disconnected"+ game_Data.reciept_game[recieptId] <<std::endl;
            return true;
        }

    }
    else if (seperated[0].compare("ERROR")==0){
        
        std::cout << "Error ocourd , the error is:" +'\n' + massage << std::endl;
        return true;
    }

    return false;
    
}
bool StompProtocol::writeToFile(std::string frame , std::string fileName){
    
    fstream my_file;
	my_file.open("../../client/data/" +fileName + ".txt", ios::out);
	if (!my_file) {
		return false;
	}
	else {
		cout << "File created successfully!" << endl;
		my_file << frame;
		my_file.close();
        return true;
	};


}