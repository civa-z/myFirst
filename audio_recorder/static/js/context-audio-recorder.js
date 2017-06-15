function (){
    ContextAudioRecorder = function(on_status_update){
        // capture audio content (not work for IE)
        var context;
        var microphone;
        var processor;
        var wavSaveWorker;

        this.onStatusUpdate = onStatusUpdate;
        this.initialize = initialize;

        function initialize(){
            navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia ||
                navigator.mozGetUserMedia || navigator.msGetUserMedia;

            if (navigator.getUserMedia) {
                navigator.getUserMedia({audio: true}, getUserMedia_successCallback, getUserMedia_errorCallback);
                return true;
            } else {
                return false;
            }
        }

        function getUserMedia_successCallback(stream){
            usingContext = true;
            context = new AudioContext();
            microphone = context.createMediaStreamSource(stream);
            processor = context.createScriptProcessor(0, 1, 1);//bufferSize大小，输入channel数，输出channel数
            processor.onaudioprocess = function (event){
                var array = event.inputBuffer.getChannelData(0);
                wavSaveWorker.postMessage({ cmd: 'ondata', buf: array });
            };

            wavSaveWorker = new Worker('static/js/worker-save-to-wav/worker-wavsaver.js');
            wavSaveWorker.onmessage = onWavSaveWorkerMessage;
            wavSaveWorker.postMessage({
                cmd: 'init', config: {sampleRate: context.sampleRate, bitRate: config.bitRate}
             });
        };

        function getUserMedia_errorCallback (error) {
            var msg;
            switch (error.code || error.name) {
                case 'PERMISSION_DENIED':
                case 'PermissionDeniedError':
                    msg = '用户拒绝访问麦客风';
                    break;
                case 'NOT_SUPPORTED_ERROR':
                case 'NotSupportedError':
                    msg = '浏览器不支持麦客风';
                    break;
                case 'MANDATORY_UNSATISFIED_ERROR':
                case 'MandatoryUnsatisfiedError':
                    msg = '找不到麦客风设备';
                    break;
                default:
                    msg = '无法打开麦克风，异常信息:' + (error.code || error.name);
                    break;
            }
            config.funCancel && config.funCancel(msg);
        };
    };
    window.ContextAudioRecorder = ContextAudioRecorder;
}();