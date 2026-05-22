import React from 'react';
import CodeBlock from './CodeBlock';

export default function Introduction({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Introduction</h1>
      <p>
        NARUTO is an interpreted programming language that executes code line-by-line through a custom runtime engine. 
        It focuses on clean syntax, predictable behavior, automatic numeric promotion, scalable object-oriented architecture, and developer readability.
      </p>

      <hr />

      <h2>Core Features</h2>
      
      <h3>Static Typing</h3>
      <p>Every variable must declare a type before usage.</p>

      <h3>Automatic Numeric Promotion</h3>
      <p>Numeric expressions automatically widen to preserve precision.</p>

      <h3>Object-Oriented Programming</h3>
      <p>Supports classes, constructors, inheritance, access modifiers, method overriding, and object instantiation.</p>

      <h3>Dynamic Arrays</h3>
      <p>Arrays automatically resize during assignment.</p>

      <h3>Multi-Dimensional Collections</h3>
      <p>Supports nested arrays of any depth.</p>

      <h3>Automatic Input Conversion</h3>
      <p>The runtime automatically converts input into numeric types when required.</p>

      <hr />

      <h2>Program Structure</h2>
      <p>
        A NARUTO program consists of variable declarations, function definitions, class definitions, expressions, and executable statements.
      </p>

      <h3>Basic Program</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`print "Welcome to NARUTO";`}
      />

      <h3>Multi-Line Program</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int age = 17;\nstring name = "Naruto";\n\nprint name;\nprint age;`}
      />

      <hr />

      <h2>Comments</h2>
      <p>Comments are ignored by the interpreter.</p>

      <h3>Single Line Comments</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`// This is a comment\nprint "Hello";`}
      />

      <h3>Multi-Line Comments</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`/*\n    This is a\n    multi-line comment\n*/\nprint "Hello";`}
      />
    </div>
  );
}
