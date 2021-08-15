// $Id: NumbersFileParser.cc 2956 2011-06-28 18:54:32Z mabl $
// ESAF : Euso Simulation and Analysis Framework
// Author: Daniel De Marco  Jan, 28 2002

/*****************************************************************************
 * ESAF: Euso Simulation and Analysis Framework                              *
 *                                                                           *
 *  Id: NumbersFileParser                                                    *
 *  Package: Base                                                            *
 *  Coordinator: Marco.Pallavicini                                           *
 *                                                                           *
 *****************************************************************************/

//_____________________________________________________________________________
//  
//   Parser of number data files 
//   ===========================
//
//   Parser of numbers data files
//   

#include "NumbersFileParser.hh"
#include "utils.hh"
#include <math.h>

ClassImp(NumbersFileParser)

//______________________________________________________________________________
NumbersFileParser::~NumbersFileParser() {
    //
    // Destructor
    //
    
    delete [] fNumbers;
    delete [] fUnits;
}

//______________________________________________________________________________
NumbersFileParser::NumbersFileParser(const string &fn, size_t ncol, Coding ft):
    fFileName(fn), fFileType(ft), fNumCol(ncol){
    // 
    // Constructor
    //

    fNumbers = new vector<Double_t>[fNumCol];
    fUnits   = new Double_t[fNumCol];

    for ( size_t i(0); i<fNumCol; i++) {
        fUnits[i] = 1;
    }

    if ( ft == gzip ) {
        // open gzip file
        gzFile zF(0);

        // check if map file or gzipped version exists
        if ( (zF = gzopen( fn.c_str(), "r" )) == 0  )
            Msg(EsafMsg::Panic) << "Unable to open file "+fn << MsgDispatch;

        const size_t max_size=500;
        Char_t line[max_size];
        string dummy;
        
        while(gzgets(zF, line, max_size) != Z_NULL ){
            dummy = line;

            const size_t row_size = dummy.size();

            if(row_size == max_size-1)
                Msg(EsafMsg::Panic) << "Row exceeds buffer in file "+fFileName << MsgDispatch;

            ProcessLine(dummy);
        }

        gzclose(zF);
    } else if ( ft == ascii ) {
        // open ascii file
        ifstream in(fFileName.c_str());
        if ( !in ) Msg(EsafMsg::Panic) << "Can't open file "+fFileName << MsgDispatch;
            
        string dummy;
        while(getline(in, dummy))
            ProcessLine(dummy);

        in.close();
    } else {
        Msg(EsafMsg::Panic) << "Unable to handle "+fFileName << MsgDispatch;
    }
}

//______________________________________________________________________________
void NumbersFileParser::ProcessLine(string line) { 
    // 
    // Helper function: gets numbers from a line
    // 

    size_t pos;

    // remove newline
    line.erase(line.find_last_not_of("\n\r")+1);

    // erase comments
    pos = line.find('#');
    if ( pos != string::npos ) 
        line.erase(pos);
            
    // swallow spaces from both sides
    line = trim(line);

    // skip blank lines
    if ( line.empty() ) 
        return;
            
    // we expect something like that:
    // number number number ...

    size_t col = 0;
    while ( !line.empty() ) {
        if(++col > fNumCol) 
            Msg(EsafMsg::Panic)  << col << " instead of " << fNumCol 
                                 << " columns found in "+fFileName << MsgDispatch;

        // get value
        const string value=line.substr(0, line.find_first_of("\t "));

        // convert to Double_t
        Double_t v = toDouble(value);
        if ( ::isnan(v) )
            FatalError(value+"is not a number");
        fNumbers[col-1].push_back(v);
            
        // erase used value
        line.erase(0, line.find_first_of("\t "));

        // since the right side of the string is already trimed, only trim the right side
        line=ltrim(line);
    } 

    if ( col != 0 && col != fNumCol )
        Msg(EsafMsg::Panic)  << col << " instead of " << fNumCol
                             << " columns found in "+fFileName << MsgDispatch;
     
}

//______________________________________________________________________________
vector<Double_t> NumbersFileParser::GetRow(size_t row) { 
    // 
    // Get a copy of a row
    // 
    
    if ( row > fNumbers[0].size()) 
        Msg(EsafMsg::Panic) << "Invalid row requested to "+fFileName << endl;
        
    vector<Double_t> dummy;

    for(size_t i=0; i<fNumCol; i++) 
        dummy.push_back(fNumbers[i][row]);

    return dummy;
}
