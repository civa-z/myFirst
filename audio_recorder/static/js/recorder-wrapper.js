$(function(){
    var RecorderWrapper = function{
        var Recorder = this;
        Recorder.onStatusUpdate = on_status_update;
        Recorder.start = start;
        Recorder.stop = stop;

        Recorder.recorder = ContextAudioRecorder(Recorder.onStatusUpdate);
        if (!Recorder.initialize())
            Recorder = none;


        function on_status_update(e){
            switch (e.status){
                case "ready":
                    break;
                case "record_finish":
                    break;
                case "error":
                    break;
            }
        }

        function start(){
            Recorder.recorder && Recorder.recorder.start();
        }

        function stop(){
            Recorder.recorder && Recorder.recorder.stop();
        }

        function on_wav_data(wavData){
            var wavBlob = new Blob(wavData, {type: "audio/wav"})
            document.getElementByID("audio_wav").src = window.createUrl(wavBlob);
        }


    }
    window.RecorderWrapper = RecorderWrapper;
});