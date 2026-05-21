import React, { useState, useEffect, useRef } from 'react';
import Editor from '@monaco-editor/react';
import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import 'xterm/css/xterm.css';

const DEFAULT_CODE = `// Naruto Language Playground
// Try changing the radius and run the code!

const float PI = 3.14;

function float calculateArea(int radius) {
    return PI * radius * radius;
}

print "Welcome to Naruto Playground!";
int radius = input("Enter radius: ");

if (radius > 0) {
    float area = calculateArea(radius);
    print "The area is: ";
    print area;
} else {
    print "Invalid radius!";
}
`;

function App() {
  const [code, setCode] = useState(DEFAULT_CODE);
  const [isRunning, setIsRunning] = useState(false);
  const terminalRef = useRef(null);
  const xtermRef = useRef(null);
  const wsRef = useRef(null);
  const fitAddonRef = useRef(null);

  useEffect(() => {
    // Initialize xterm.js
    const term = new Terminal({
      theme: {
        background: '#0d1117',
        foreground: '#c9d1d9',
        cursor: '#58a6ff',
      },
      fontFamily: '"Fira Code", monospace',
      fontSize: 14,
      convertEol: true, // convert \n to \r\n automatically
    });
    const fitAddon = new FitAddon();
    term.loadAddon(fitAddon);
    
    term.open(terminalRef.current);
    fitAddon.fit();
    term.writeln('--- NARUTO INTERACTIVE TERMINAL ---');
    term.writeln('Ready to execute code...\r\n');

    xtermRef.current = term;
    fitAddonRef.current = fitAddon;

    // Handle user typing into the terminal
    term.onData((data) => {
      // Send user keystrokes to the backend via WebSocket
      if (wsRef.current && wsRef.current.readyState === WebSocket.OPEN) {
        // xterm.js sends \r when Enter is pressed. We need to send \n to the backend process.
        const inputData = data === '\r' ? '\n' : data;
        wsRef.current.send(JSON.stringify({ type: 'input', input: inputData }));
        
        // Echo back to terminal
        if (data === '\r') {
            term.write('\r\n');
        } else if (data === '\x7f') { // Handle backspace visual
            term.write('\b \b');
        } else {
            term.write(data);
        }
      }
    });

    const handleResize = () => fitAddon.fit();
    window.addEventListener('resize', handleResize);

    return () => {
      window.removeEventListener('resize', handleResize);
      term.dispose();
      if (wsRef.current) {
        wsRef.current.close();
      }
    };
  }, []);

  const handleRun = () => {
    if (isRunning) return;
    setIsRunning(true);
    
    xtermRef.current.clear();
    xtermRef.current.writeln('Running...\r\n');

    // Connect to WebSocket server
    // If running locally, connect to backend port 3001. If on Render, use the same host!
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1'
        ? 'ws://localhost:3001'
        : `${protocol}//${window.location.host}`;
        
    const ws = new WebSocket(wsUrl);
    wsRef.current = ws;

    ws.onopen = () => {
      ws.send(JSON.stringify({ type: 'start', code }));
    };

    ws.onmessage = (event) => {
      const msg = JSON.parse(event.data);
      if (msg.type === 'output') {
        // Output from stdout/stderr of the program
        // We replace standard newlines with \r\n for xterm to render correctly, but convertEol handles it mostly
        xtermRef.current.write(msg.data);
      } else if (msg.type === 'exit') {
        xtermRef.current.writeln(`\r\n\r\n[Process exited with code ${msg.code}]`);
        setIsRunning(false);
      }
    };

    ws.onerror = (error) => {
      xtermRef.current.writeln('\\r\\nWebSocket error. Is the backend running?');
      setIsRunning(false);
    };

    ws.onclose = () => {
      setIsRunning(false);
    };
  };

  return (
    <div className="playground-container">
      <header className="header">
        <div className="logo">🍥 Naruto Playground</div>
        <button className="run-button" onClick={handleRun} disabled={isRunning}>
          {isRunning ? 'Running...' : 'Run Code'}
        </button>
      </header>
      
      <div className="main-content">
        <div className="editor-pane">
          <div className="pane-title">Code</div>
          <Editor
            height="calc(100vh - 100px)"
            defaultLanguage="c"
            theme="vs-dark"
            value={code}
            onChange={(value) => setCode(value || '')}
            options={{
              minimap: { enabled: false },
              fontSize: 14,
              fontFamily: '"Fira Code", monospace',
            }}
          />
        </div>
        
        <div className="terminal-pane">
          <div className="pane-title">Interactive Terminal</div>
          <div className="xterm-container" ref={terminalRef}></div>
        </div>
      </div>
    </div>
  );
}

export default App;
