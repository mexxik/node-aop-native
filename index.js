const addon = require('./build/Release/node_aop.node');

module.exports = {
    create: function() {
        return new addon.AOP();
    }
};