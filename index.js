var sync_prompt = require("./build/Release/sync_prompt.node");


module.exports = function(prompt, options) {
    if (prompt) process.stdout.write(prompt);

    options = options || {};
    
    var setEcho = !options.secure;
    sync_prompt.set_echo(setEcho);

    var res = sync_prompt.sync_prompt();
    sync_prompt.set_echo(false);

    return res;
}
