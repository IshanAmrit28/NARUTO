import React from 'react';
import CodeBlock from './CodeBlock';

export default function OOP({ onTryCode }) {
  return (
    <div className="docs-page">
      <h1>Object-Oriented Programming (OOP)</h1>

      <h2>Classes</h2>
      <p>A Class is a blueprint for creating objects. It encapsulates data (fields) and behavior (methods).</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    string name;\n    int chakra;\n}`}
      />

      <h2>Objects</h2>
      <p>An Object is an instance of a Class, created using the <code>new</code> keyword.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    string name;\n}\n\nNinja naruto = new Ninja();\nnaruto.name = "Naruto";\nprint naruto.name;`}
      />

      <hr />

      <h2>Constructor</h2>
      <p>Constructors run when an object is created and must be named <code>init()</code>.</p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    string name;\n\n    function void init(string n) {\n        this.name = n;\n    }\n}\n\nNinja n = new Ninja("Naruto");\nprint n.name;`}
      />

      <h2>Destructor (Garbage Collection)</h2>
      <p>
        Unlike C++, NARUTO is a memory-managed language. You do not need to explicitly write a destructor. 
        The runtime's Garbage Collector automatically cleans up objects and frees memory when they are no longer in use.
      </p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`function void performJutsu() {\n    Ninja clone = new Ninja("Shadow Clone");\n    // 'clone' is automatically destroyed and memory is freed when the function ends!\n}`}
      />

      <hr />

      <h2>Encapsulation</h2>
      <p>
        Encapsulation is the bundling of data and the methods that act on that data, while restricting direct access to some components using <code>private</code>.
      </p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    private int chakra;\n\n    function void init(int c) {\n        this.chakra = c;\n    }\n\n    // Public getter to safely access private data\n    function int getChakra() {\n        return this.chakra;\n    }\n}\n\nNinja n = new Ninja(100);\nprint n.getChakra();\n// n.chakra would cause an error!`}
      />

      <hr />

      <h2>Inheritance</h2>
      <p>
        Inheritance allows a child class to inherit fields and methods from a parent class using <code>extends</code>.
        Use <code>super()</code> to call the parent's constructor.
      </p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    string name;\n\n    function void init(string n) {\n        this.name = n;\n    }\n}\n\nclass Shinobi extends Ninja {\n    function void init(string n) {\n        super(n);\n    }\n}\n\nShinobi sasuke = new Shinobi("Sasuke");\nprint sasuke.name;`}
      />

      <hr />

      <h2>Polymorphism</h2>
      <p>
        Polymorphism allows child classes to override parent methods to provide specific implementations.
      </p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Ninja {\n    function void speak() {\n        print "Ninja";\n    }\n}\n\nclass Shinobi extends Ninja {\n    function void speak() {\n        print "Elite Ninja";\n    }\n}\n\nShinobi sasuke = new Shinobi();\nsasuke.speak(); // Prints "Elite Ninja"`}
      />

      <hr />

      <h2>Abstraction</h2>
      <p>
        Abstraction is the concept of hiding complex internal implementation details and exposing only the essential features through a simple public interface.
      </p>
      <CodeBlock 
        onTryCode={onTryCode}
        code={`class Rasengan {\n    private int rotationSpeed;\n    private int chakraDensity;\n\n    function void init() {\n        this.rotationSpeed = 0;\n        this.chakraDensity = 0;\n    }\n\n    private void shapeChakra() {\n        this.rotationSpeed = 9000;\n    }\n\n    private void compressChakra() {\n        this.chakraDensity = 100;\n    }\n\n    // Public Interface: The user doesn't need to know HOW it works internally!\n    function void execute() {\n        this.shapeChakra();\n        this.compressChakra();\n        print "Rasengan Executed!";\n    }\n}\n\nRasengan attack = new Rasengan();\nattack.execute();`}
      />
    </div>
  );
}
