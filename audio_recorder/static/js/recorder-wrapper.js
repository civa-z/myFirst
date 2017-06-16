$(function(){
    var RecorderWrapper = this;
    RecorderWrapper.onStatusUpdate = on_status_update;
    RecorderWrapper.start = start;
    RecorderWrapper.stop = stop;
    RecorderWrapper.registerWavDataCallback = register_wav_data_callback;

    RecorderWrapper.recorder = ContextAudioRecorder(RecorderWrapper.onStatusUpdate);
    if (!RecorderWrapper.recorder.initialize())
        RecorderWrapper.recorder = none;

    // test data start
    var context_recorder = document.getElementById("context-recorder");
    context_recorder.onmousedown = RecorderWrapper.start;
    context_recorder.onmouseup = RecorderWrapper.stop;
    RecorderWrapper.registerWavDataCallback(on_wav_data);

    function on_wav_data(){
        var wavData = RecorderWrapper.recorder.getWavData();
        var wavBlob = new Blob([wavData], {type: "audio/wav"});
        var wavUrl = window.URL.createObjectURL(wavBlob);
        document.getElementById("audio-wav").src = wavUrl;
    }
    // test data end

    function on_status_update(e){
        switch (e.status){
            case "ready":
                console.log("RecorderWrapper on_status_update ready" );
                break;
            case "record_finish":
                console.log("RecorderWrapper on_status_update record_finish" );
                RecorderWrapper.onWavData();
                break;
            case "error":
                console.log("RecorderWrapper on_status_update error: " + e.status);
                break;
        }
    }

    function start(){
        console.log("RecorderWrapper start");
        RecorderWrapper.recorder && RecorderWrapper.recorder.start();
    }

    function stop(){
        console.log("RecorderWrapper stop");
        RecorderWrapper.recorder && RecorderWrapper.recorder.stop();
    }

    function register_wav_data_callback(onWavData){
        console.log("RecorderWrapper register_wav_data_callback");
        RecorderWrapper.onWavData = onWavData;
    }
});