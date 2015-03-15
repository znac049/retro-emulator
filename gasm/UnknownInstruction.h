#ifndef _UNKNOWN_INSTRUCTION_
#define _UNKNOWN_INSTRUCTION_

#include <stdexcept>

class UnknownInstruction : public std::runtime_error {
 public:
  /** Constructor (C strings).
   *  @param message C-style string error message.
   *                 The string contents are copied upon construction.
   *                 Hence, responsibility for deleting the \c char* lies
   *                 with the caller. 
   */
  UnknownInstruction(const char* message) : std::runtime_error(message) { }
};

#endif
