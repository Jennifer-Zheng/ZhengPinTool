
/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs 
 *  and could serve as the starting point for developing your first PIN tool
 */

#include "pin.H"
#include <iostream>
#include <fstream>
using std::ofstream;
using std::ios;
using std::cerr;
using std::string;
using std::endl;

ofstream Outfile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;
static UINT64 regCount = 0;
static UINT64 immCount = 0;
static UINT64 dispCount = 0;
static UINT64 regIndCount = 0;
static UINT64 indexedCount = 0;
static UINT64 directCount = 0;
static UINT64 memIndCount = 0;
static UINT64 autoIncCount = 0;
static UINT64 autoDecCount = 0;
static UINT64 scaledCount = 0;

// This function is called before every instruction is executed
// VOID docount() { icount++;}
// VOID doRegCount() { regCount++;}
// VOID doImmCount() { immCount++;}
// VOID doDispCount() { dispCount++;}
// VOID doRegIndCount() { regIndCount++;}
// VOID doIndexedCount() { indexedCount++;}
// VOID doDirectCount() { directCount++;}
// VOID doMemIndCount() { memIndCount++;}
// VOID doAutoIncCount() { autoIncCount++;}
// VOID doAutoDecCount() { autoDecCount++;}
// VOID doScaledCount() { scaledCount++;}

    
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Go over operands
    INT32 count = INS_OperandCount(ins);
    for (INT32 i = 0; i < count; i++) {

        if (INS_OperandIsImmediate(ins, i)) {
            immCount++;
        } else if (INS_OperandIsBranchDisplacement(ins, i)) {
            dispCount++;
        }
    }
    
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "inscount.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Count " << icount << endl;
    OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}

// /* ================================================================== */
// // Global variables 
// /* ================================================================== */

// UINT64 insCount = 0;        //number of dynamically executed instructions
// UINT64 bblCount = 0;        //number of dynamically executed basic blocks
// UINT64 threadCount = 0;     //total number of threads, including main thread

// std::ostream * out = &cerr;

// /* ===================================================================== */
// // Command line switches
// /* ===================================================================== */
// KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,  "pintool",
//     "o", "", "specify file name for MyPinTool output");

// KNOB<BOOL>   KnobCount(KNOB_MODE_WRITEONCE,  "pintool",
//     "count", "1", "count instructions, basic blocks and threads in the application");


// /* ===================================================================== */
// // Utilities
// /* ===================================================================== */

// /*!
//  *  Print out help message.
//  */
// INT32 Usage()
// {
//     cerr << "This tool prints out the number of dynamically executed " << endl <<
//             "instructions, basic blocks and threads in the application." << endl << endl;

//     cerr << KNOB_BASE::StringKnobSummary() << endl;

//     return -1;
// }

// /* ===================================================================== */
// // Analysis routines
// /* ===================================================================== */

// /*!
//  * Increase counter of the executed basic blocks and instructions.
//  * This function is called for every basic block when it is about to be executed.
//  * @param[in]   numInstInBbl    number of instructions in the basic block
//  * @note use atomic operations for multi-threaded applications
//  */
// VOID CountBbl(UINT32 numInstInBbl)
// {
//     bblCount++;
//     insCount += numInstInBbl;
// }

// /* ===================================================================== */
// // Instrumentation callbacks
// /* ===================================================================== */

// /*!
//  * Insert call to the CountBbl() analysis routine before every basic block 
//  * of the trace.
//  * This function is called every time a new trace is encountered.
//  * @param[in]   trace    trace to be instrumented
//  * @param[in]   v        value specified by the tool in the TRACE_AddInstrumentFunction
//  *                       function call
//  */
// VOID Trace(TRACE trace, VOID *v)
// {
//     // Visit every basic block in the trace
//     for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
//     {
//         // Insert a call to CountBbl() before every basic bloc, passing the number of instructions
//         BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)CountBbl, IARG_UINT32, BBL_NumIns(bbl), IARG_END);
//     }
// }

// /*!
//  * Increase counter of threads in the application.
//  * This function is called for every thread created by the application when it is
//  * about to start running (including the root thread).
//  * @param[in]   threadIndex     ID assigned by PIN to the new thread
//  * @param[in]   ctxt            initial register state for the new thread
//  * @param[in]   flags           thread creation flags (OS specific)
//  * @param[in]   v               value specified by the tool in the 
//  *                              PIN_AddThreadStartFunction function call
//  */
// VOID ThreadStart(THREADID threadIndex, CONTEXT *ctxt, INT32 flags, VOID *v)
// {
//     threadCount++;
// }

// /*!
//  * Print out analysis results.
//  * This function is called when the application exits.
//  * @param[in]   code            exit code of the application
//  * @param[in]   v               value specified by the tool in the 
//  *                              PIN_AddFiniFunction function call
//  */
// VOID Fini(INT32 code, VOID *v)
// {
//     *out <<  "===============================================" << endl;
//     *out <<  "MyPinTool analysis results: " << endl;
//     *out <<  "Number of instructions: " << insCount  << endl;
//     *out <<  "Number of basic blocks: " << bblCount  << endl;
//     *out <<  "Number of threads: " << threadCount  << endl;
//     *out <<  "===============================================" << endl;
// }

// /*!
//  * The main procedure of the tool.
//  * This function is called when the application image is loaded but not yet started.
//  * @param[in]   argc            total number of elements in the argv array
//  * @param[in]   argv            array of command line arguments, 
//  *                              including pin -t <toolname> -- ...
//  */
// int main(int argc, char *argv[])
// {
//     // Initialize PIN library. Print help message if -h(elp) is specified
//     // in the command line or the command line is invalid 
//     if( PIN_Init(argc,argv) )
//     {
//         return Usage();
//     }
    
//     string fileName = KnobOutputFile.Value();

//     if (!fileName.empty()) { out = new std::ofstream(fileName.c_str());}

//     if (KnobCount)
//     {
//         // Register function to be called to instrument traces
//         TRACE_AddInstrumentFunction(Trace, 0);

//         // Register function to be called for every thread before it starts running
//         PIN_AddThreadStartFunction(ThreadStart, 0);

//         // Register function to be called when the application exits
//         PIN_AddFiniFunction(Fini, 0);
//     }
    
//     cerr <<  "===============================================" << endl;
//     cerr <<  "This application is instrumented by MyPinTool" << endl;
//     if (!KnobOutputFile.Value().empty()) 
//     {
//         cerr << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
//     }
//     cerr <<  "===============================================" << endl;

//     // Start the program, never returns
//     PIN_StartProgram();
    
//     return 0;
// }

// /* ===================================================================== */
// /* eof */
// /* ===================================================================== */
