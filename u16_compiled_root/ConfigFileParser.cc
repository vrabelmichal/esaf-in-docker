// ESAF : Euso Simulation and Analysis Framework
// $Id: ConfigFileParser.cc 2956 2011-06-28 18:54:32Z mabl $
// Daniel De Marco created Jan, 28 2002

#include "ConfigFileParser.hh"
#include "EsafSys.hh"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <TSystem.h>

ClassImp(ConfigFileParser)

string ConfigFileParser::fCfgDir = "./config";

//______________________________________________________________________________
ConfigFileParser::ConfigFileParser(const string &type, const string &s, const string &fn) 
    : fBasicType(type), fBasicName(s) {
    
    // 
    // Constructor
    //

    if(fn.empty()) fPath=type+"/"+s+".cfg";
    else fPath=fn;

    ParseConfigFile();
}

//______________________________________________________________________________
void ConfigFileParser::CheckId(const string &s) {

    if(fNumbers.count(s)!=0 || fChars.count(s)!=0)
        cout<<"warning: multiple definition of "<<s<<endl;
}

//______________________________________________________________________________
string ConfigFileParser::GetStrPath(const string &s) { 
    //
    // returns a string performing path expansion
    //
    static string str;
    static TString str2;
    str = GetStr(s);

    if ( !gEsafSys )
        return str;

    // copy in the TString (this is insane)
    str2 = gEsafSys->AbsolutePath(str.c_str());
    gSystem->ExpandPathName(str2);
    str = str2.Data();
    return str;
}

//______________________________________________________________________________
Bool_t ConfigFileParser::GetBool(const string &s) { 
    //
    // return a bool
    //
    static string str;
    str = GetStr(s);

    if ( str == "yes" ) return kTRUE;
    else if ( str == "no" ) return kFALSE;
    else {
        Msg(EsafMsg::Error) << "In file " << fCfgDir+"/"+fPath << MsgDispatch;
        FatalError("Value "+s+" must be yes or no");
    }

    return kFALSE;
}



//______________________________________________________________________________
void ConfigFileParser::ParseConfigFile() {
    //
    // Parse the config file
    //

    // open config file
    string fname = fCfgDir+"/"+fPath;
    ifstream in(fname.c_str());
    if(!in) Msg(EsafMsg::Panic) << "can't open file "+fPath << MsgDispatch;
    
    string dummy;
    while(getline(in, dummy))
        ParseLine(dummy);

}

//______________________________________________________________________________
void ConfigFileParser::ParseLine( string line) {
        size_t pos;

        // erase comments
        pos=line.find('#');
        if(pos!=string::npos) line.erase(pos);

        // swallow spaces
        line=ltrim(line);

        // skip blank lines
        if(line.empty()) return;

        // we expect something like that:
        // identifier=number
        // identifier=string

        // get identifier
        string identifier=line.substr(0,line.find('='));
        identifier=rtrim(identifier);
        if(identifier.find_first_of("\t ")!=string::npos) 
            Msg(EsafMsg::Panic) << "wrong assignment: "+line << MsgDispatch;

        // get value
        string value=line.substr(line.find('=')+1, string::npos);
        value=ltrim(value);
        value=rtrim(value);
        // trying for a double
        char *p;
        double d=strtod(value.c_str(), &p);
        if((d==0 && value.c_str()==p)  || ::isinf(d) || ::isnan(d))  {
            // not a double, inf and nan are not considered numbers
            CheckId(identifier);
            fChars[identifier]=value; 
        } else { 
            // a double
            CheckId(identifier);
            fNumbers[identifier]=d; 
        }

}

//______________________________________________________________________________
void ConfigFileParser::ReplaceStr(const string &s, const string& val) {
    // 
    // Replace a string value in the Config
    //

    string s1 = s;
    s1 = ltrim(s1);
    if (s1.empty()) 
        Msg(EsafMsg::Panic) << "invalid key: "+s << MsgDispatch;
    fChars[s] = val;
}

//______________________________________________________________________________
void ConfigFileParser::ReplaceNum(const string &s, const double val) {
    // 
    // Replace a numerical value in the Config
    // 

    string s1 = s;
    s1 = ltrim(s1);
    if (s1.empty()) 
        Msg(EsafMsg::Panic) << "invalid key: "+s << MsgDispatch;
    fNumbers[s1] = val;
}

//______________________________________________________________________________
void ConfigFileParser::Replace(const string &s, Bool_t add) {
    //
    // Replace a string or a number value in the Config
    // 

    string s1 = s;
    s1 = ltrim(s1);
    if (s1.empty()) 
        Msg(EsafMsg::Panic) << "invalid expr: "+s << MsgDispatch;

    string identifier=s1.substr(0,s1.find('='));

    identifier=rtrim(identifier);
    if(identifier.find_first_of("\t ")!=string::npos) 
        Msg(EsafMsg::Panic) << "wrong assignment: "+s1 << MsgDispatch;

    if (!add && fChars.count(identifier) == 0 && fNumbers.count(identifier) == 0)
        Msg(EsafMsg::Panic) << identifier+" does not exists in class "+fBasicName << MsgDispatch;

    // get value
    string value=s1.substr(s1.find('=')+1, string::npos);
    value=ltrim(value);
    value=rtrim(value);
    // trying for a double
    char *p;
    double d=strtod(value.c_str(), &p);
    if(d==0 && value.c_str()==p) { // not a double
        fChars[identifier]=value; 
    } else { // a double
        fNumbers[identifier]=d; 
    }
}


//______________________________________________________________________________
void ConfigFileParser::Write(const string &name) const {
    //
    // Write the content of this to file
    //

    // open file
    ofstream out(name.c_str());
    if(!out) Msg(EsafMsg::Panic) << "can't open file "+fPath << MsgDispatch;

    char datestr[100];
    time_t timet=time(0);
    strftime(datestr, 100, "%Y-%m-%d %H:%M", localtime(&timet));
    // write header
    out<<"# "<<name<<"\n"
        <<"# original filename: "<<fPath<<"\n"
        <<"# "<<datestr<<endl;

    // write fNumbers
    map<string, double>::const_iterator i=fNumbers.begin();
    for(; i!=fNumbers.end(); i++) {
        out<<i->first<<" = "<<i->second<<endl;
    }

    // write strings
    map<string, string>::const_iterator p=fChars.begin();
    for(; p!=fChars.end(); p++) {
        out<<p->first<<" = "<<p->second<<endl;
    }
}
