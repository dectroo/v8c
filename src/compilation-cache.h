// Copyright 2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_COMPILATION_CACHE_H_
#define V8_COMPILATION_CACHE_H_

namespace v8 { namespace internal {


// The compilation cache keeps function boilerplates for compiled
// scripts and evals. The boilerplates are looked up using the source
// string as the key.
class CompilationCache {
 public:
  // The same source code string has different compiled code for
  // scripts and evals. Internally, we use separate caches to avoid
  // getting the wrong kind of entry when looking up.
  enum Entry {
    SCRIPT,
    EVAL_GLOBAL,
    EVAL_CONTEXTUAL
  };

  // Finds the script function boilerplate for a source
  // string. Returns an empty handle if the cache doesn't contain a
  // script for the given source string with the right origin.
  static Handle<JSFunction> LookupScript(Handle<String> source,
                                         Handle<Object> name,
                                         int line_offset,
                                         int column_offset);

  // Finds the function boilerplate for a source string for
  // eval. Returns an empty handle if the cache doesn't contain a
  // script for the given source string.
  static Handle<JSFunction> LookupEval(Handle<String> source,
                                       Entry entry);

  // Associate the (source, kind) pair to the boilerplate. This may
  // overwrite an existing mapping.
  static void Associate(Handle<String> source,
                        Entry entry,
                        Handle<JSFunction> boilerplate);

  // Clear the cache - also used to initialize the cache at startup.
  static void Clear();

  // GC support.
  static void Iterate(ObjectVisitor* v);

  // Notify the cache that a mark-sweep garbage collection is about to
  // take place. This is used to retire entries from the cache to
  // avoid keeping them alive too long without using them. For now, we
  // just clear the cache but we should consider are more
  // sophisticated LRU scheme.
  static void MarkCompactPrologue() { Clear(); }
};


} }  // namespace v8::internal

#endif  // V8_COMPILATION_CACHE_H_
