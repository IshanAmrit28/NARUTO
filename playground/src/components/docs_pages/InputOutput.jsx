import React from 'react';
import CodeBlock from './CodeBlock';

export default function InputOutput({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Input & Output</h1>

      <h2>Output</h2>
      <p>The <code>print</code> keyword outputs expressions to the console.</p>

      <h3>Example</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`print "Hello";\nprint 100;\nprint true;`}
      />

      <h3>String Concatenation</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`string name = "Naruto";\nint age = 17;\n\nprint name + " is " + age;`}
      />

      <hr />

      <h2>Input</h2>
      <p>The <code>input()</code> function reads user input dynamically.</p>

      <h3>String Input Example</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`string name = input("Enter name: ");\n\nprint name;`}
      />

      <h3>Integer Input</h3>
      <p>Assigning input to an integer automatically converts the user's string text into a number.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int age = input("Enter age: ");\n\nprint age;`}
      />

      <h3>Float Input</h3>
      <p>Assigning input to a float automatically converts the user's string text into a decimal.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`float weight = input("Enter weight: ");\n\nprint weight;`}
      />
    </div>
  );
}
