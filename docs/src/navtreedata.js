/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Maestro Audio Framework", "index.html", [
    [ "Getting started with Maestro framework", "index.html", [
      [ "Introduction", "index.html#autotoc_md1", null ],
      [ "Supported examples", "index.html#autotoc_md2", [
        [ "maestro_playback", "index.html#autotoc_md3", null ],
        [ "maestro_record", "index.html#autotoc_md4", null ],
        [ "maestro_usb_mic", "index.html#autotoc_md5", null ],
        [ "maestro_usb_speaker", "index.html#autotoc_md6", null ],
        [ "maestro_sync", "index.html#autotoc_md7", null ]
      ] ],
      [ "Example applications overview", "index.html#autotoc_md8", null ],
      [ "Commands", "index.html#autotoc_md9", [
        [ "version", "index.html#autotoc_md10", null ],
        [ "file", "index.html#autotoc_md11", null ],
        [ "record_mic", "index.html#autotoc_md12", null ],
        [ "opus encode", "index.html#autotoc_md13", null ],
        [ "usb_mic", "index.html#autotoc_md14", null ],
        [ "usb_speaker", "index.html#autotoc_md15", null ],
        [ "start", "index.html#autotoc_md16", null ]
      ] ],
      [ "Configuration options", "index.html#autotoc_md17", null ],
      [ "Supported features", "index.html#autotoc_md18", [
        [ "Codecs", "index.html#autotoc_md19", null ],
        [ "VIT", "index.html#autotoc_md20", null ],
        [ "VoiceSeeker", "index.html#autotoc_md21", null ],
        [ "EAP", "index.html#autotoc_md22", null ],
        [ "SSRC", "index.html#autotoc_md23", null ]
      ] ]
    ] ],
    [ "Maestro Audio Framework Programmer's Guide", "md_ProgrammersGuide.html", [
      [ "Introduction", "md_ProgrammersGuide.html#autotoc_md25", null ],
      [ "Architecture overview", "md_ProgrammersGuide.html#autotoc_md26", null ],
      [ "Pipeline", "md_ProgrammersGuide.html#autotoc_md27", null ],
      [ "Elements and Pads", "md_ProgrammersGuide.html#autotoc_md28", null ],
      [ "Internal communication", "md_ProgrammersGuide.html#autotoc_md29", null ],
      [ "Decoders and encoders", "md_ProgrammersGuide.html#autotoc_md30", [
        [ "Common codec interface", "md_ProgrammersGuide.html#autotoc_md31", null ],
        [ "Using the CCI to interface with Metadata", "md_ProgrammersGuide.html#autotoc_md32", null ],
        [ "Using the CCI to interface with Decoders", "md_ProgrammersGuide.html#autotoc_md33", null ],
        [ "Adding new decoders to the CCI", "md_ProgrammersGuide.html#autotoc_md34", null ],
        [ "Common encoder interface", "md_ProgrammersGuide.html#autotoc_md35", null ]
      ] ],
      [ "Maestro performance", "md_ProgrammersGuide.html#autotoc_md36", [
        [ "Memory information", "md_ProgrammersGuide.html#autotoc_md37", null ],
        [ "CPU usage", "md_ProgrammersGuide.html#autotoc_md38", null ]
      ] ],
      [ "Opus codec", "md_ProgrammersGuide.html#autotoc_md39", null ],
      [ "Opus file", "md_ProgrammersGuide.html#autotoc_md40", null ]
    ] ],
    [ "CEI encoder", "md_CeiEncoder.html", [
      [ "Header files", "md_CeiEncoder.html#autotoc_md42", null ],
      [ "Instantiating the element", "md_CeiEncoder.html#autotoc_md43", null ],
      [ "Element properties", "md_CeiEncoder.html#autotoc_md44", null ],
      [ "CEI definition - implementing your own encoder", "md_CeiEncoder.html#autotoc_md45", null ]
    ] ],
    [ "AAC decoder", "aacdec_start.html", [
      [ "AAC decoder features", "aacdec_start.html#autotoc_md46", null ],
      [ "Specification and reference", "aacdec_start.html#autotoc_md47", null ],
      [ "Performance", "aacdec_start.html#autotoc_md48", [
        [ "Memory information", "aacdec_start.html#autotoc_md49", null ],
        [ "CPU usage", "aacdec_start.html#autotoc_md50", null ]
      ] ]
    ] ],
    [ "FLAC decoder", "flacdec_start.html", [
      [ "FLAC decoder features", "flacdec_start.html#autotoc_md51", null ],
      [ "Specification and reference", "flacdec_start.html#autotoc_md52", [
        [ "Official website", "flacdec_start.html#autotoc_md53", null ],
        [ "Inbound licensing", "flacdec_start.html#autotoc_md54", null ]
      ] ],
      [ "Performance", "flacdec_start.html#autotoc_md55", [
        [ "Memory information", "flacdec_start.html#autotoc_md56", null ],
        [ "CPU usage", "flacdec_start.html#autotoc_md57", null ],
        [ "Following test cases are performed:", "flacdec_start.html#autotoc_md58", null ]
      ] ]
    ] ],
    [ "MP3 decoder", "mp3dec_start.html", [
      [ "MP3 decoder features", "mp3dec_start.html#autotoc_md59", null ],
      [ "Performance", "mp3dec_start.html#autotoc_md60", [
        [ "Memory information", "mp3dec_start.html#autotoc_md61", null ],
        [ "CPU usage", "mp3dec_start.html#autotoc_md62", null ]
      ] ]
    ] ],
    [ "WAV decoder", "md_wavdec.html", [
      [ "WAV decoder features", "md_wavdec.html#autotoc_md64", null ],
      [ "Performance", "md_wavdec.html#autotoc_md65", [
        [ "Memory information", "md_wavdec.html#autotoc_md66", null ],
        [ "CPU usage", "md_wavdec.html#autotoc_md67", null ],
        [ "Following test cases were performed:", "md_wavdec.html#autotoc_md68", null ]
      ] ]
    ] ],
    [ "Modules", "modules.html", "modules" ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"LVC__Types_8h.html",
"audio__sink__pcmrtos_8h.html#a043254bd941a12bd9c487f39e3d74990",
"decoder_8h.html#a10aab45d4e85d74dffe02b5e0b27ded2",
"globals_defs_y.html",
"pad_8h.html#a0557c37b7b3093294af1c6794979cca0",
"streamer__api_8h.html#a1abb7fc632ea61f43d927e80d7a39958",
"streamer__element__properties_8h.html#a3813dce788312ca7db2f66b1392ac236",
"structElementEncoder.html#a1daa4bc6b8a6731330eabf55d56a82f0",
"struct__ElementAudioSink.html#abb904fd48c04c976b54fb47d04b4414c",
"structringbuf.html#af43bcf56233301d218aa6042e5457623"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';