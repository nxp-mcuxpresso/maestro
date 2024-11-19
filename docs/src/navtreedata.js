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
    [ "Introduction", "index.html#autotoc_md1", null ],
    [ "Maestro on Zephyr", "index.html#autotoc_md2", null ],
    [ "Maestro on FreeRTOS", "index.html#autotoc_md3", [
      [ "Supported examples", "index.html#supported-examples", null ],
      [ "Example applications overview", "index.html#autotoc_md4", null ]
    ] ],
    [ "File structure", "index.html#autotoc_md5", null ],
    [ "Contribution to the Maestro project", "index.html#autotoc_md6", null ],
    [ "Maestro Audio Framework Programmer's Guide", "md_ProgrammersGuide.html", [
      [ "Introduction", "md_ProgrammersGuide.html#autotoc_md8", null ],
      [ "Architecture overview", "md_ProgrammersGuide.html#autotoc_md9", null ],
      [ "Pipeline", "md_ProgrammersGuide.html#autotoc_md10", null ],
      [ "Elements", "md_ProgrammersGuide.html#autotoc_md11", [
        [ "Add a new element type", "md_ProgrammersGuide.html#autotoc_md12", null ],
        [ "Add a new element index", "md_ProgrammersGuide.html#autotoc_md13", null ]
      ] ],
      [ "Pads", "md_ProgrammersGuide.html#autotoc_md14", null ],
      [ "Internal communication", "md_ProgrammersGuide.html#autotoc_md15", null ],
      [ "Decoders and encoders", "md_ProgrammersGuide.html#autotoc_md16", [
        [ "Common codec interface", "md_ProgrammersGuide.html#autotoc_md17", null ],
        [ "Using the CCI to interface with Metadata", "md_ProgrammersGuide.html#autotoc_md18", null ],
        [ "Using the CCI to interface with Decoders", "md_ProgrammersGuide.html#autotoc_md19", null ],
        [ "Adding new decoders to the CCI", "md_ProgrammersGuide.html#autotoc_md20", null ],
        [ "Common encoder interface", "md_ProgrammersGuide.html#autotoc_md21", null ]
      ] ],
      [ "Maestro performance", "md_ProgrammersGuide.html#autotoc_md22", [
        [ "Memory information", "md_ProgrammersGuide.html#autotoc_md23", null ],
        [ "CPU usage", "md_ProgrammersGuide.html#autotoc_md24", null ]
      ] ]
    ] ],
    [ "CEI encoder", "md_CeiEncoder.html", [
      [ "Header files", "md_CeiEncoder.html#autotoc_md26", null ],
      [ "Instantiating the element", "md_CeiEncoder.html#autotoc_md27", null ],
      [ "Element properties", "md_CeiEncoder.html#autotoc_md28", null ],
      [ "CEI definition - implementing your own encoder", "md_CeiEncoder.html#autotoc_md29", null ]
    ] ],
    [ "Maestro playback example", "md_maestro__playback.html", [
      [ "Table of content", "md_maestro__playback.html#autotoc_md31", null ],
      [ "Overview", "md_maestro__playback.html#maestro-playback-overview", null ],
      [ "Hardware requirements", "md_maestro__playback.html#maestro-playback-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__playback.html#maestro-playback-HW-modifications", null ],
      [ "Preparation", "md_maestro__playback.html#maestro-playback-preparation", null ],
      [ "Running the demo", "md_maestro__playback.html#maestro-playback-running-the-demo", null ],
      [ "Example configuration", "md_maestro__playback.html#maestro-playback-example-configuration", null ],
      [ "Functionality", "md_maestro__playback.html#maestro-playback-functionality", null ],
      [ "States", "md_maestro__playback.html#maestro-playback-states", null ],
      [ "Commands in detail", "md_maestro__playback.html#maestro-playback-commands-in-detail", [
        [ "help, version", "md_maestro__playback.html#maestro-playback-help-version", null ],
        [ "file stop", "md_maestro__playback.html#file-stop", null ],
        [ "file pause", "md_maestro__playback.html#file-pause", null ],
        [ "file volume <volume>", "md_maestro__playback.html#file-volume-volume", null ],
        [ "file seek <seek_time>", "md_maestro__playback.html#file-seek-seek_time", null ],
        [ "file play <filename>", "md_maestro__playback.html#file-play-filename", null ],
        [ "file list", "md_maestro__playback.html#file-list", null ],
        [ "file info", "md_maestro__playback.html#file-info", null ]
      ] ],
      [ "Processing Time", "md_maestro__playback.html#maestro-playback-processing-time", null ]
    ] ],
    [ "Maestro record example", "md_maestro__record.html", [
      [ "Table of content", "md_maestro__record.html#autotoc_md33", null ],
      [ "Overview", "md_maestro__record.html#maestro-record-overview", null ],
      [ "Hardware requirements", "md_maestro__record.html#maestro-record-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__record.html#maestro-record-HW-modifications", null ],
      [ "Preparation", "md_maestro__record.html#maestro-record-preparation", null ],
      [ "Running the demo", "md_maestro__record.html#maestro-record-running-the-demo", null ],
      [ "Example configuration", "md_maestro__record.html#maestro-record-example-configuration", null ],
      [ "Functionality", "md_maestro__record.html#maestro-record-functionality", null ],
      [ "States", "md_maestro__record.html#maestro-record-states", null ],
      [ "Commands in detail", "md_maestro__record.html#maestro-record-commands-in-detail", [
        [ "help, version", "md_maestro__record.html#maestro-record-help-version", null ],
        [ "record_mic audio <time>", "md_maestro__record.html#maestro-record-record-mic-audio", null ],
        [ "record_mic file <time> ; record_mic <file_name> <time>", "md_maestro__record.html#maestro-record-record-mic-file", null ],
        [ "record_mic vit <time> <language>", "md_maestro__record.html#maestro-record-record-mic-vit", null ],
        [ "opus_encode", "md_maestro__record.html#maestro-record-opus_encode", null ]
      ] ],
      [ "Processing Time", "md_maestro__record.html#maestro-record-processing-time", null ]
    ] ],
    [ "Maestro USB microphone example", "md_maestro__usb__mic.html", [
      [ "Table of content", "md_maestro__usb__mic.html#autotoc_md35", null ],
      [ "Overview", "md_maestro__usb__mic.html#maestro-usb-mic-overview", null ],
      [ "Hardware requirements", "md_maestro__usb__mic.html#maestro-usb-mic-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__usb__mic.html#maestro-usb-mic-HW-modifications", null ],
      [ "Preparation", "md_maestro__usb__mic.html#maestro-usb-mic-preparation", null ],
      [ "Running the demo", "md_maestro__usb__mic.html#maestro-usb-mic-running-the-demo", null ],
      [ "Example configuration", "md_maestro__usb__mic.html#maestro-usb-mic-example-configuration", null ],
      [ "Functionality", "md_maestro__usb__mic.html#maestro-usb-mic-functionality", null ],
      [ "States", "md_maestro__usb__mic.html#maestro-usb-mic-states", null ],
      [ "Commands in detail", "md_maestro__usb__mic.html#maestro-usb-mic-commands-in-detail", [
        [ "help, version", "md_maestro__usb__mic.html#maestro-usb-mic-help-version", null ],
        [ "usb_mic <seconds>", "md_maestro__usb__mic.html#maestro-usb-mic-usb-mic", null ]
      ] ]
    ] ],
    [ "Maestro USB speaker example", "md_maestro__usb__speaker.html", [
      [ "Table of content", "md_maestro__usb__speaker.html#autotoc_md37", null ],
      [ "Overview", "md_maestro__usb__speaker.html#maestro-usb-speaker-overview", null ],
      [ "Hardware requirements", "md_maestro__usb__speaker.html#maestro-usb-speaker-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__usb__speaker.html#maestro-usb-speaker-HW-modifications", null ],
      [ "Preparation", "md_maestro__usb__speaker.html#maestro-usb-speaker-preparation", null ],
      [ "Running the demo", "md_maestro__usb__speaker.html#maestro-usb-speaker-running-the-demo", null ],
      [ "Example configuration", "md_maestro__usb__speaker.html#maestro-usb-speaker-example-configuration", null ],
      [ "Functionality", "md_maestro__usb__speaker.html#maestro-usb-speaker-functionality", null ],
      [ "States", "md_maestro__usb__speaker.html#maestro-usb-speaker-states", null ],
      [ "Commands in detail", "md_maestro__usb__speaker.html#maestro-usb-speaker-commands-in-detail", [
        [ "help, version", "md_maestro__usb__speaker.html#maestro-usb-speaker-help-version", null ],
        [ "usb_speaker <seconds>", "md_maestro__usb__speaker.html#maestro-usb-speaker-usb-speaker", null ]
      ] ]
    ] ],
    [ "Maestro sync example", "md_maestro__sync.html", [
      [ "Table of content", "md_maestro__sync.html#autotoc_md39", null ],
      [ "Overview", "md_maestro__sync.html#maestro-sync-overview", null ],
      [ "Hardware requirements", "md_maestro__sync.html#maestro-sync-HW-requirements", null ],
      [ "Hardware modifications", "md_maestro__sync.html#maestro-sync-HW-modifications", null ],
      [ "Preparation", "md_maestro__sync.html#maestro-sync-preparation", null ],
      [ "Running the demo", "md_maestro__sync.html#maestro-sync-running-the-demo", null ],
      [ "Example configuration", "md_maestro__sync.html#maestro-sync-example-configuration", null ],
      [ "Functionality", "md_maestro__sync.html#maestro-sync-functionality", null ],
      [ "States", "md_maestro__sync.html#maestro-sync-states", null ],
      [ "Commands in detail", "md_maestro__sync.html#maestro-sync-commands-in-detail", [
        [ "help, version", "md_maestro__sync.html#maestro-sync-help-version", null ],
        [ "start [nosdcard]", "md_maestro__sync.html#maestro-sync-start", null ],
        [ "stop", "md_maestro__sync.html#maestro-sync-stop", null ],
        [ "debug [on|off]", "md_maestro__sync.html#maestro-sync-debug-on-off", null ]
      ] ]
    ] ],
    [ "Supported features", "md_supported__features.html", [
      [ "Decoders", "md_supported__features.html#autotoc_md41", null ],
      [ "Encoders", "md_supported__features.html#autotoc_md42", null ],
      [ "Sample rate converters", "md_supported__features.html#autotoc_md43", null ],
      [ "Additional libraries", "md_supported__features.html#autotoc_md44", null ]
    ] ],
    [ "Processing Time", "md_processing__time.html", [
      [ "Table of content", "md_processing__time.html#autotoc_md46", null ],
      [ "Maestro playback example", "md_processing__time.html#maestro-playback-times", [
        [ "Time on each element", "md_processing__time.html#autotoc_md47", null ]
      ] ],
      [ "Maestro record example", "md_processing__time.html#maestro-record-times", [
        [ "Pipeline Microphone -> Speaker", "md_processing__time.html#autotoc_md48", null ],
        [ "Pipeline Microphone -> VIT", "md_processing__time.html#autotoc_md49", null ],
        [ "Pipeline Microphone -> Voice seeker -> VIT", "md_processing__time.html#autotoc_md50", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"index.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';