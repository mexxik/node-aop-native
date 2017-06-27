var AOP = require('./../index');

exports.group = {
    setUp: function(callback) {
        this.object = {
            functionSimple: function() {
                console.log('invoking original functionSimple\n');
            },
            functionWithArgs: function(arg1, arg2, arg3) {
                console.log('invoking original functionWithArgs, args: %s, %s, %s', arg1, arg2, arg3);
            },
            functionWithObjectArg: function(arg) {
                console.log('invoking original functionWithObjectArg, arg property: %d', arg.property);
            }
        };

        this.args = {
            arg1: 'a1',
            arg2: 'a2',
            arg3: 'a3'
        };

        this.value = {
            property: 1
        };

        callback();
    },
    testSimple: function(test) {
        console.log('===============================');
        console.log('==========testSimple===========');

        var aop = AOP.create();
        aop.before('functionSimple', this.object, function() {
            console.log('functionSimple intercepted\n');

            test.done();
        });

        this.object.functionSimple();
        console.log('===============================\n\n');
    },

    testWithArgs: function(test) {
        console.log('===============================');
        console.log('=========testWithArgs==========');

        var self = this;

        var aop = AOP.create();
        aop.before('functionWithArgs', self.object, function(arg1, arg2, arg3) {
            console.log('functionWithArgs intercepted, args: %s, %s, %s', arg1, arg2, arg3);
            test.equal(arg1, self.args.arg1, 'arg1 is not equal');
            test.equal(arg2, self.args.arg2, 'arg2 is not equal');
            test.equal(arg3, self.args.arg3, 'arg3 is not equal');
            test.done();
        });

        self.object.functionWithArgs(self.args.arg1, self.args.arg2, self.args.arg3);
        console.log('===============================\n\n');
    },

    testWithArgsMutation: function(test) {
        console.log('===============================');
        console.log('=====testWithArgsMutation======');

        var self = this;


        var aop = AOP.create();
        aop.before('functionWithObjectArg', self.object, function(arg) {
            console.log('functionWithObjectArg intercepted, arg property: %d', arg.property);

            arg.property = 2;
            test.done();
        });

        self.object.functionWithObjectArg(self.value);
        console.log('===============================\n\n');
    }
}