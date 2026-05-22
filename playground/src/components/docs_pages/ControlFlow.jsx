import React from 'react';
import CodeBlock from './CodeBlock';

export default function ControlFlow({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Control Flow & Operators</h1>

      <h2>Arithmetic Operators</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int a = 10;\nint b = 3;\n\nprint a + b; // 13\nprint a - b; // 7\nprint a * b; // 30\nprint a / b; // 3\nprint a % b; // 1`}
      />

      <h2>Compound Assignment</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int x = 10;\n\nx += 5; // x is 15\nx -= 2; // x is 13\nx *= 3; // x is 39\nx /= 3; // x is 13\nprint x;`}
      />

      <h2>Increment & Decrement</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int value = 5;\n\nvalue++; // 6\nvalue--; // 5\nprint value;`}
      />

      <h2>Relational Operators</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`print 10 > 5;   // true\nprint 10 < 5;   // false\nprint 10 == 10; // true\nprint 10 != 5;  // true`}
      />

      <h2>Logical Operators</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`bool a = true;\nbool b = false;\n\nprint a && b; // false\nprint a || b; // true\nprint !a;     // false`}
      />

      <h2>Bitwise Operators</h2>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int flags = 5;\n\nprint flags & 1;  // Bitwise AND\nprint flags | 2;  // Bitwise OR\nprint flags ^ 3;  // Bitwise XOR\nprint flags << 1; // Left Shift\nprint flags >> 1; // Right Shift`}
      />

      <hr />

      <h2>Conditional Statements</h2>

      <h3>if Statement</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int chakra = 50;\n\nif (chakra > 40) {\n    print "Ready";\n}`}
      />

      <h3>if else Statement</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int chakra = 20;\n\nif (chakra > 40) {\n    print "Ready";\n} else {\n    print "Rest";\n}`}
      />

      <h3>else if Chain</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int level = 3;\n\nif (level == 1) {\n    print "Genin";\n} else if (level == 2) {\n    print "Chunin";\n} else {\n    print "Jonin";\n}`}
      />

      <h3>Switch Statement</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int rank = 2;\n\nswitch (rank) {\n    case 1:\n        print "Genin";\n    case 2:\n        print "Chunin";\n    default:\n        print "Unknown";\n}`}
      />

      <hr />

      <h2>Loops</h2>

      <h3>While Loop</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int i = 0;\n\nwhile (i < 5) {\n    print i;\n    i++;\n}`}
      />

      <h3>For Loop</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`for (int i = 0; i < 5; i++) {\n    print i;\n}`}
      />

      <h3>Break Statement</h3>
      <p>Use <code>break</code> to exit a loop early.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`for (int i = 0; i < 10; i++) {\n    if (i == 5) {\n        break;\n    }\n    print i;\n}`}
      />

      <h3>Continue Statement</h3>
      <p>Use <code>continue</code> to skip the rest of the current iteration.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`for (int i = 0; i < 5; i++) {\n    if (i == 2) {\n        continue;\n    }\n    print i;\n}`}
      />
    </div>
  );
}
