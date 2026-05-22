import React from 'react';
import CodeBlock from './CodeBlock';

export default function DataStructures({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Arrays & Structs</h1>

      <h2>Arrays</h2>
      <p>Arrays must be declared with a single data type using <code>[]</code>.</p>

      <h3>Array Declaration</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [1, 2, 3];`}
      />

      <h3>Accessing Elements</h3>
      <p>Access elements via their zero-based index.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [1, 2, 3];\nprint numbers[0];`}
      />

      <h3>Updating Elements</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [1, 2, 3];\nnumbers[1] = 99;\nprint numbers[1];`}
      />

      <h3>Dynamic Expansion</h3>
      <p>Assigning beyond the array's current size will expand it automatically.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [1, 2, 3];\nnumbers[5] = 100;\nprint numbers[5];`}
      />

      <h3>Push Method</h3>
      <p>Use <code>.push()</code> to append a value to the end of an array.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [];\nnumbers.push(500);\nprint numbers[0];`}
      />

      <h3>Array Length</h3>
      <p>Get the current size of the array.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[] numbers = [1, 2, 3];\nprint numbers.length;`}
      />

      <hr />

      <h2>Multi-Dimensional Arrays</h2>
      <p>Arrays can be nested to any depth.</p>

      <h3>Declaration</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[][] matrix = [\n    [1, 2],\n    [3, 4]\n];\n\nprint matrix[0][1]; // Prints 2`}
      />

      <h3>Nested Push</h3>
      <p>You can mutate elements inside nested arrays seamlessly.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`int[][] matrix = [[1, 2], [3, 4]];\nmatrix[1].push(99);\nprint matrix[1][2]; // Prints 99`}
      />

      <hr />

      <h2>Structs</h2>
      <p>Structs are lightweight data containers. They cannot have methods or inheritance.</p>

      <h3>Struct Syntax</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`struct Point {\n    int x;\n    int y;\n}`}
      />

      <h3>Struct Instantiation</h3>
      <p>Structs are instantiated without <code>new</code>. They use a positional argument syntax.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`struct Point {\n    int x;\n    int y;\n}\n\nPoint p = Point(10, 20);\nprint p.x;`}
      />

      <h3>Struct With Arrays</h3>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`struct Team {\n    string name;\n    int[] scores;\n}\n\nTeam t = Team("Leaf", [10, 20]);\nt.scores.push(30);\nprint t.scores[2];`}
      />
    </div>
  );
}
