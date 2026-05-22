import React from 'react';
import CodeBlock from './CodeBlock';

export default function Functions({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Functions</h1>

      <h2>Function Syntax</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        runnable={false}
        code={`function returnType functionName(parameters) {\n\n}`}
      />

      <h2>Simple Function</h2>
      <p>Use <code>void</code> if the function returns nothing.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`function void greet() {\n    print "Hello";\n}\n\ngreet();`}
      />

      <h2>Function With Parameters</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`function void welcome(string name) {\n    print "Welcome " + name;\n}\n\nwelcome("Naruto");`}
      />

      <h2>Function With Return Value</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`function int add(int a, int b) {\n    return a + b;\n}\n\nint result = add(10, 20);\nprint result;`}
      />

      <h2>Recursive Function</h2>
      <p>Functions can call themselves.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`function int factorial(int n) {\n    if (n <= 1) {\n        return 1;\n    }\n    return n * factorial(n - 1);\n}\n\nprint factorial(5);`}
      />
    </div>
  );
}
