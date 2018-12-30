# grammar

```js

class A extends B {
    constructor(first, second) {
        this.first = first;
        this.second = second;
    }

    method(name) {
        return new A(name);
    }

    get fullname() {
        return this.first + this.second;
    }

    static static_method(first, second) {
        return new A(first, second);
    }
}

function test_func(a, b = 1) {
    return a + b;
}

async function test_async(a ,b){
    return await a(b);
}

const test_arrow = (a, b) => a + b;

var async_arrow = async (n) => await func(n);

while(1) {
    let a = a ? 0 : 1;
    lex.output(a);
    await delay(100);
}

```