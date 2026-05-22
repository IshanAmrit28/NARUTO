import React from 'react';
import CodeBlock from './CodeBlock';

export default function VariablesAndTypes({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Variables & Data Types</h1>

      <h2>What is a Variable?</h2>
      <p>
        A variable is a named memory location used to store data during execution.
        Each variable contains a type, a name, and a value.
      </p>

      <h3>Variable Declaration Syntax</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        runnable={false}
        code={`datatype variableName = value;`}
      />

      <h3>Example</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int age = 16;\nstring village = "Leaf";\nbool isNinja = true;\n\nprint age;\nprint village;\nprint isNinja;`}
      />

      <hr />

      <h2>Integer Types</h2>
      
      <h3>byte</h3>
      <p><strong>Description:</strong> An 8-bit signed integer. Range: -128 to 127</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`byte level = 10;\nprint level;`}
      />

      <h3>short</h3>
      <p><strong>Description:</strong> A 16-bit signed integer.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`short score = 1200;\nprint score;`}
      />

      <h3>int</h3>
      <p><strong>Description:</strong> A 32-bit signed integer.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int chakra = 9000;\nprint chakra;`}
      />

      <h3>long</h3>
      <p><strong>Description:</strong> A 64-bit signed integer.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`long worldPopulation = 8000000000;\nprint worldPopulation;`}
      />

      <hr />

      <h2>Floating Point Types</h2>
      
      <h3>float</h3>
      <p><strong>Description:</strong> Stores decimal numbers with standard precision.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`float speed = 12.5;\nprint speed;`}
      />

      <h3>double</h3>
      <p><strong>Description:</strong> Stores high precision decimal numbers.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`double gravity = 9.80665;\nprint gravity;`}
      />

      <hr />

      <h2>Text Types</h2>
      
      <h3>string</h3>
      <p><strong>Description:</strong> Stores sequences of text characters.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`string name = "Sasuke";\nprint name;`}
      />

      <h3>char</h3>
      <p><strong>Description:</strong> Stores a single character.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`char rank = 'A';\nprint rank;`}
      />

      <hr />

      <h2>Boolean Type</h2>
      
      <h3>bool</h3>
      <p><strong>Description:</strong> Stores either true or false.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`bool alive = true;\nprint alive;`}
      />

      <hr />

      <h2>Type System</h2>
      <p>NARUTO uses static typing. A variable cannot change its structural type after declaration.</p>

      <h3>Valid Assignment</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int power = 100;\npower = 200;\nprint power;`}
      />

      <h3>Numeric Promotion</h3>
      <p>When numeric types interact, the runtime automatically promotes the result to the most precise type.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int a = 10;\nfloat b = 2.5;\n\n// a (int) is implicitly coerced to float\nfloat result = a + b;\n\nprint result;`}
      />

      <hr />

      <h2>Type Casting</h2>
      <p>Explicit casting converts one type into another type.</p>

      <h3>Float to Integer</h3>
      <p>Truncates the decimal value.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`float damage = 99.9;\nint finalDamage = int(damage);\n\nprint finalDamage; // Prints 99`}
      />

      <h3>Number to String</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int id = 404;\nstring msg = string(id);\n\nprint msg;`}
      />

      <h3>String to Integer</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`string value = "500";\nint power = int(value);\n\nprint power;`}
      />
    </div>
  );
}
