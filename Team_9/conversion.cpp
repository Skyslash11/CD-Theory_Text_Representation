#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <string>
#include <sstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

int parser(string line);
string sentenceFormulation(string iD);
string actionFormulation(string agent,string action);

map<string,pair<string,int> > eventMap;
map<string,pair<string,int> > stateMap;
vector<vector<string> > precedeVec;
vector<vector<string> > sametimeVec;
vector<vector<string> > resultVec;
vector<vector<string> > reasonVec; 
vector<vector<string> > totalorder;

main()
{
	ifstream infile("CDTheory");
	string line;
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    if(line.length()==0)
	    	continue;
	    //cout<<line<<"\n";
	    int test=parser(line);
	    //if(test==1)
	    //	cout<<"ggwp\n";
	}
	string story="";
	for(int i=0;i<totalorder.size();++i)
	{
    	for(int j=0;j<totalorder[i].size();++j)
    	{
    		cout<<totalorder[i][j]<<"  ";
    	}
    	cout<<"\n";
    }
    for(int i=0;i<9;++i)
	{
    	if(totalorder[i].size()==1)
    	{
    		story+=sentenceFormulation(totalorder[i][0]);
    		story+='.';
    	}
    	else if(totalorder[i].size()==3)
    	{
    		if(totalorder[i][2]=="sametime")
    		{
    			story+=sentenceFormulation(totalorder[i][0]);
    			story+=" and ";
    			story+=sentenceFormulation(totalorder[i][1]);
    			story+='.';
    		}
    		else if(totalorder[i][2]=="reason")
    		{
    			story+=sentenceFormulation(totalorder[i][0]);
    			story+=" because ";
    			story+=sentenceFormulation(totalorder[i][1]);
    			story+='.';
    		}
    		else if(totalorder[i][2]=="result")
    		{
    			story+=sentenceFormulation(totalorder[i][0]);
    			story+=" as a result of which ";
    			story+=sentenceFormulation(totalorder[i][1]);
    			story+='.';
    		}
    	}
    }

    cout<<story<<"\n";
}

int parser(string line)
{
	if(line[0]=='(' && line[1]==' ' && line[line.length()-1]==')' && line[line.length()-2]==' ')
	{
		//cout<< line[line.length()-1]<<" \n";
	}
	else
		return 0;
	string sub1=line.substr(2,line.length()-4);
	string subevent=sub1.substr(0,5);
	string subad=sub1.substr(0,11);
	string substate=sub1.substr(0,5);
	string subst=sub1.substr(0,8);
	string subprecede=sub1.substr(0,7);
	string subreason=sub1.substr(0,6);
	string subresult=sub1.substr(0,6);

	if(subevent=="event")
	{
		//cout<<"event\n";
		string event=line.substr(8,line.length()-10);
		string eventID="";
		int counter=0;
		while(event[counter]!=' ')
		{
			eventID+=event[counter];
			++counter;
		}
		event=event.substr(eventID.length()+3,event.length()-eventID.length()-5);	
		//cout<<eventID<<"________\n";
		pair<string,int> tempPair;
		tempPair.first=event;
		tempPair.second=0;
		eventMap[eventID]=tempPair;
		if(totalorder.size()==0)
		{
			vector<string> tempVec;
			tempVec.push_back(eventID);
			//cout<<"__________\n";
			totalorder.push_back(tempVec);
		}
		return 1;
	}
	else if(substate=="state")
	{
		//cout<<"state\n";
		string state=line.substr(8,line.length()-10);
		string stateID="";
		int counter=0;
		while(state[counter]!=' ')
		{
			stateID+=state[counter];
			++counter;
		}
		state=state.substr(stateID.length()+3,state.length()-stateID.length()-5);	
		//cout<<state<<"\n";
		pair<string,int> tempPair;
		tempPair.first=state;
		tempPair.second=0;
		stateMap[stateID]=tempPair;
		if(totalorder.size()==0)
		{
			vector<string> tempVec;
			tempVec.push_back(stateID);
			totalorder.push_back(tempVec);
		}
		return 1;
	}
	else if(subst=="sametime")
	{
		string sametime=line.substr(11,line.length()-13);
		string ID1="";
		string ID2="";
		int counter=0;
		while(sametime[counter]!=' ')
		{
			ID1+=sametime[counter];
			++counter;
		}
		++counter;
		while(sametime[counter]!='\0')
		{
			ID2+=sametime[counter];
			++counter;
		}
		//cout<<ID1<<"   "<<ID2<<"\n";
		for(int i=0;i<totalorder.size();++i)
		{
			if(totalorder[i][0]==ID1)
			{
				totalorder[i].push_back(ID2);
				totalorder[i].push_back("sametime");
			}
			else if(totalorder[i][0]==ID2)
			{
				totalorder[i].push_back(totalorder[i][0]);
				totalorder[i][0]=ID1;
				totalorder[i].push_back("sametime");
			}
		}
		return 1;
	}
	else if(subprecede=="precede")
	{
		string precede=line.substr(10,line.length()-12);
		string ID1="";
		string ID2="";
		int counter=0;
		while(precede[counter]!=' ')
		{
			ID1+=precede[counter];
			++counter;
		}
		++counter;
		while(precede[counter]!='\0')
		{
			ID2+=precede[counter];
			++counter;
		}
		//cout<<ID1<<"   "<<ID2<<"\n";
		for(int i=0;i<totalorder.size();++i)
		{
			int match=0;
			for(int j=0;j<totalorder[i].size();++j)
			{
				if(totalorder[i][j]==ID1)
					match=1;
			}
			if(match==1)
			{
				if(totalorder.size()==i+1)
				{
					vector<string> tempVec;
					tempVec.push_back(ID2);
					totalorder.push_back(tempVec);
				}
				else
				{
					vector<string> tempVec;
					tempVec.push_back(ID2);
					totalorder.insert(totalorder.begin()+i+1,tempVec);
				}
				break;
			}
			else
			{
					for(int j=0;j<totalorder[i].size();++j)
					{
						if(totalorder[i][j]==ID2)
						{
							vector<string> tempVec;
							tempVec.push_back(ID1);
							totalorder.insert(totalorder.begin()+i,tempVec);
							match=1;
							break;
						}
					}
					if(match==1)
						break;
			}
		}
		return 1;
	}
	else if(subreason=="reason")
	{
		string reason=line.substr(9,line.length()-11);
		string ID1="";
		string ID2="";
		int counter=0;
		while(reason[counter]!=' ')
		{
			ID1+=reason[counter];
			++counter;
		}
		++counter;
		while(reason[counter]!='\0')
		{
			ID2+=reason[counter];
			++counter;
		}
		//cout<<ID1<<"   "<<ID2<<"\n";
		for(int i=0;i<totalorder.size();++i)
		{
			if(totalorder[i][0]==ID1)
			{
				totalorder[i].push_back(ID2);
				totalorder[i].push_back("reason");
			}
			else if(totalorder[i][0]==ID2)
			{
				totalorder[i].push_back(totalorder[i][0]);
				totalorder[i][0]=ID1;
				totalorder[i].push_back("reason");
			}
		}
		return 1;
	}
	else if(subresult=="result")
	{
		string result=line.substr(9,line.length()-11);
		string ID1="";
		string ID2="";
		int counter=0;
		while(result[counter]!=' ')
		{
			ID1+=result[counter];
			++counter;
		}
		++counter;
		while(result[counter]!='\0')
		{
			ID2+=result[counter];
			++counter;
		}
		//cout<<ID1<<"   "<<ID2<<"\n";
		for(int i=0;i<totalorder.size();++i)
		{
			if(totalorder[i][0]==ID1)
			{
				totalorder[i].push_back(ID2);
				totalorder[i].push_back("result");
			}
			else if(totalorder[i][0]==ID2)
			{
				totalorder[i].push_back(totalorder[i][0]);
				totalorder[i][0]=ID1;
				totalorder[i].push_back("result");
			}
		}
		return 1;
	}
	else
		return 0;
}


string sentenceFormulation(string iD)
{
	//cout<<iD<<"  $$ "<<stateMap[iD].first<<"\n";
	if(eventMap.count(iD)==1)
	{
		string st=eventMap[iD].first;
		if(st[0]=='d' && st[1]=='o')
		{
			string agent="";
			int counter=3;
			while(st[counter]!=' ')
			{
				agent+=st[counter];
				++counter;
			}
			string action=""; 
			action=st.substr(6+agent.length(),st.length()-8-agent.length());
			//cout<<agent<<"   "<<action<<"\n";
			string ret=actionFormulation(agent,action);
			return ret;
		}
		else
		{
			string arg1="";
			string arg2="";
			string arg3="";
			int counter=0;
			while(st[counter]!=' ')
			{
				arg1+=st[counter];
				++counter;
			}
			++counter;
			while(st[counter]!=' ')
			{
				arg2+=st[counter];
				++counter;
			}
			++counter;
			while(st[counter]!='\0')
			{
				arg3+=st[counter];
				++counter;
			}
			//cout<<arg1<<"  "<<arg2<<"  "<<arg3<<"\n";
			string ret="";
			if(arg3.compare("...")!=0)
		    {
			ret.append(arg2);
			ret.append(" ");
			ret.append(arg1);
			ret.append(" ");
			ret.append(arg3);
			//ret+=arg2+' '+arg1+' '+arg3;
			}
			else
			{
			ret.append(arg2);
			ret.append(" ");
			ret.append(arg1);
			}
			return ret;
		}
		return "event";
	}
	else if(stateMap.count(iD)==1)
	{
		//cout<<"----------------\n";
		string st=stateMap[iD].first;
		string arg1="";
		string arg2="";
		string arg3="";
		int counter=0;
		while(st[counter]!=' ')
		{
			arg1+=st[counter];
			++counter;
		}
		++counter;
		while(st[counter]!=' ')
		{
			arg2+=st[counter];
			++counter;
		}
		++counter;
		while(st[counter]!='\0')
		{
			arg3+=st[counter];
			++counter;
		}
		//cout<<arg1<<"  "<<arg2<<"  "<<arg3<<"\n";
		string ret="";
		if(arg3.compare("...")!=0 && arg1.compare("behaviour")==0)
		{
			ret.append(arg2);
			ret.append(" was ");
			ret.append(arg3);
			//ret+=arg2+" was "+arg3;
		}	
		else if(arg3.compare("...")!=0 && arg1.compare("behaviour")!=0)
		{
			ret.append(arg2);
			ret.append(" ");
			ret.append(arg1);
			ret.append(" ");
			ret.append(arg3);
			//ret+=arg2+' '+arg1+' '+arg3;
		}
		else
		{
			ret.append(arg2);
			ret.append(" ");
			ret.append(arg1);
		}
		return ret;
	}
	else
		return "...";
}	

string actionFormulation(string agent,string action)
{
	string ret="";
	string mtrans=action.substr(0,6);
	string mbuild=action.substr(0,6);
	string move=action.substr(0,4);
	string attend=action.substr(0,6);
	string notattend=action.substr(0,9);
	if(mtrans.compare("MTRANS")==0)
	{
		//cout<<"I am here 1 \n";
		//cout<<"st is : "<<action<<"\n";
		string st=action.substr(7,st.length()-7);
		string arg1="";
		string arg2="";
		string arg3="";
		string arg4="";
		int counter=0;
		//cout<<"st is : "<<st<<"\n";
		while(st[counter]!=' ')
		{
			arg1+=st[counter];
			++counter;
		}
		counter+=3;
		while(st[counter]!=' ' || st[counter+1]!=')')
		{
			arg2+=st[counter];
			++counter;
		}
		counter+=3;
		if(st[counter]=='.' && st[counter+1]=='.' && st[counter+2]=='.')
			arg3="...";
		else
		{
			counter+=2;
			while(st[counter]!=' ' || st[counter+1]!=')')
			{
				arg3+=st[counter];
				++counter;
			}
		}

		//cout<<arg1<<"___"<<arg2<<"___"<<arg3<<"\n";
		string speaker="";
		string listener="";
		counter=0;
		while(arg2[counter]!=' ')
		{
			//arg1+=st[counter];
			++counter;
		}
		++counter;
		while(arg2[counter]!='\0')
		{
			speaker+=arg2[counter];
			++counter;
		}
		//cout<<"Speaker is : "<<speaker<<"\n";
		counter=0;
		if(arg3.compare("...")==0)
			listener="...";
		else
		{
			while(arg3[counter]!=' ')
			{
				//arg1+=st[counter];
				++counter;
			}
			++counter;
			while(arg3[counter]!='\0')
			{
				listener+=arg3[counter];
				++counter;
			}
		}
		//cout<<"Speaker is : "<<speaker.length()<<"    Listener is : "<<listener.length()<<"  telling : "<<sentenceFormulation("st-7")<<"\n";
		//string ret="";
		string speech=sentenceFormulation(arg1);
		//cout<<speech<<"||||\n";
		//string ret;
		ret=speaker;
		if(listener.compare("...")==0)
		{
			//speech.append(speaker);
			ret.append(" told that ");
			//ret.append(listener);
			ret.append(speech);
			//cout<<ret<<"\n";
		}
		else
		{
			//speech.append(speaker);
			ret.append(" told ");
			ret.append(listener);
			ret.append(" that ");
			ret.append(speech);
			//cout<<ret<<"\n";
		}	
		return ret;
		//return "ggwp";
	}
	else if(mbuild.compare("MBUILD")==0)
	{
		string st=action.substr(9,action.length()-11);
		//cout<<st<<".,,,,,,,,,,,\n";
		string arg1="";
		string arg2="";
		string arg3="";
		int counter=0;
		while(st[counter]!=' ')
		{
			arg1+=st[counter];
			++counter;
		}
		++counter;
		while(st[counter]!=' ')
		{
			arg2+=st[counter];
			++counter;
		}
		++counter;
		while(st[counter]!='\0')
		{
			arg3+=st[counter];
			++counter;
		}

		return agent+" decided to "+arg1+" "+arg3;
	}
	else if(move=="MOVE")
	{
		string st=action.substr(5,action.length()-5);
		//cout<<st<<".,,,,,,,,,,,\n";
		string arg1="";
		string arg2="";
		int counter=0;
		while(st[counter]!=' ')
		{
			arg1+=st[counter];
			++counter;
		}
		++counter;
		while(st[counter]!='\0')
		{
			arg2+=st[counter];
			++counter;
		}
		return agent+" reached "+arg2;
	}
	else if(attend=="attend")
	{
		string st=action.substr(12,action.length()-12);
		string arg1="";
		string arg2="";
		if(st[0]=='(' && st[st.length()-1]==')')
		{
			st=st.substr(2,st.length()-4);
			int counter=0;
			while(st[counter]!=' ')
			{
				arg1+=st[counter];
				++counter;
			}
			++counter;
			while(st[counter]!='\0')
			{
				arg2+=st[counter];
				++counter;
			}

			if(arg1.compare("loc")==0)
				ret=agent+" saw "+arg2+" location";
			else
				ret=agent+" saw "+arg2+" "+arg1;
		}
		else
		{
			arg1=st;
			if(arg1.compare("world")==0)
				ret=agent+" woke up";
			else
				ret=agent+" saw "+arg1;
		}
		return ret;
	}
	else if(notattend=="notattend")
	{
		string st=action.substr(15,action.length()-15);
		string arg1="";
		string arg2="";
		if(st[0]=='(' && st[st.length()-1]==')')
		{
			st=st.substr(2,st.length()-4);
			int counter=0;
			while(st[counter]!=' ')
			{
				arg1+=st[counter];
				++counter;
			}
			++counter;
			while(st[counter]!='\0')
			{
				arg2+=st[counter];
				++counter;
			}

			if(arg1.compare("loc")==0)
				ret=agent+" did not see "+arg2+" location";
			else
				ret=agent+" did not see "+arg2+" "+arg1;
		}
		else
		{
			arg1=st;
			if(arg1.compare("world")==0)
				ret=agent+" did not wake up";
			else
				ret=agent+" did not see "+arg1;
		}
		return ret;
	}
	return "agent did action";	
}