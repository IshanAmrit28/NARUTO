import React, { useState, useEffect, useRef } from 'react';
import { Analytics } from "@vercel/analytics/react";
import Editor from '@monaco-editor/react';
import { Terminal } from 'xterm';
import { FitAddon } from 'xterm-addon-fit';
import 'xterm/css/xterm.css';
import Docs from './components/Docs';

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
  const [view, setView] = useState('docs'); // 'docs' | 'playground'
  const [mainCode, setMainCode] = useState(DEFAULT_CODE);
  
  // If tryCode is a string, the modal is open.
  const [tryCode, setTryCode] = useState(null); 
  const [isRunning, setIsRunning] = useState(false);
  
  const terminalRef = useRef(null);
  const xtermRef = useRef(null);
  const wsRef = useRef(null);
  const fitAddonRef = useRef(null);

  const isTerminalVisible = view === 'playground' || tryCode !== null;

  useEffect(() => {
    // Only initialize the terminal when a view requiring it is active
    if (!isTerminalVisible || !terminalRef.current) return;

    // Initialize xterm.js
    const term = new Terminal({
      theme: {
        background: '#0d1117',
        foreground: '#c9d1d9',
        cursor: '#ff8c00',
      },
      fontFamily: '"Fira Code", monospace',
      fontSize: 14,
      convertEol: true,
    });
    const fitAddon = new FitAddon();
    term.loadAddon(fitAddon);
    
    term.open(terminalRef.current);
    
    setTimeout(() => {
      try {
        if (fitAddon.proposeDimensions()) {
          fitAddon.fit();
        }
      } catch (e) {
        console.warn('xterm fit error:', e);
      }
    }, 50);
    
    term.writeln('--- NARUTO INTERACTIVE TERMINAL ---');
    term.writeln('Ready to execute code...\r\n');

    xtermRef.current = term;
    fitAddonRef.current = fitAddon;

    term.onData((data) => {
      if (wsRef.current && wsRef.current.readyState === WebSocket.OPEN) {
        const inputData = data === '\r' ? '\n' : data;
        wsRef.current.send(JSON.stringify({ type: 'input', input: inputData }));
        
        if (data === '\r') {
            term.write('\r\n');
        } else if (data === '\x7f') {
            term.write('\b \b');
        } else {
            term.write(data);
        }
      }
    });

    const resizeObserver = new ResizeObserver(() => {
      try {
        if (fitAddon.proposeDimensions()) {
          fitAddon.fit();
        }
      } catch (e) {
        console.warn('xterm resize fit error:', e);
      }
    });

    resizeObserver.observe(terminalRef.current);

    return () => {
      resizeObserver.disconnect();
      term.dispose();
      if (wsRef.current) {
        wsRef.current.close();
      }
    };
  }, [isTerminalVisible]);

  const handleRun = () => {
    if (isRunning) return;
    setIsRunning(true);
    
    if (xtermRef.current) {
        xtermRef.current.clear();
        xtermRef.current.writeln('Running...\r\n');
    }

    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const autoDetectedUrl = window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1'
        ? 'ws://localhost:3001'
        : `${protocol}//${window.location.host}`;
        
    let wsUrl = import.meta.env.VITE_BACKEND_WS_URL || autoDetectedUrl;
    
    if (window.location.protocol === 'https:') {
        wsUrl = wsUrl.trim().replace(/^ws:\/\//i, 'wss://');
    }
        
    const ws = new WebSocket(wsUrl);
    wsRef.current = ws;

    // Use tryCode if modal is open, else mainCode
    const currentCodeToRun = tryCode !== null ? tryCode : mainCode;

    ws.onopen = () => {
      ws.send(JSON.stringify({ type: 'start', code: currentCodeToRun }));
    };

    ws.onmessage = (event) => {
      const msg = JSON.parse(event.data);
      if (msg.type === 'output') {
        if (xtermRef.current) xtermRef.current.write(msg.data);
      } else if (msg.type === 'exit') {
        if (xtermRef.current) xtermRef.current.writeln(`\r\n\r\n[Process exited with code ${msg.code}]`);
        setIsRunning(false);
      }
    };

    ws.onerror = (error) => {
      if (xtermRef.current) xtermRef.current.writeln('\r\nWebSocket error. Is the backend running?');
      setIsRunning(false);
    };

    ws.onclose = () => {
      setIsRunning(false);
    };
  };

  const handleTryCode = (code) => {
    setTryCode(code); // Opens modal
  };

  const closeTryModal = () => {
    setTryCode(null);
  };

  return (
    <div className="app-container">
      <Analytics />
      <header className="main-header">
        <div className="logo-section">
          <span className="logo-icon">🍥</span>
          <span className="logo-text">Naruto</span>
        </div>
        
        <nav className="top-nav">
          <button 
            className={`nav-btn ${view === 'docs' ? 'active' : ''}`}
            onClick={() => {
              setView('docs');
              setTryCode(null); // Ensure modal is closed if navigating away
            }}
          >
            Documentation
          </button>
          <button 
            className={`nav-btn ${view === 'playground' ? 'active' : ''}`}
            onClick={() => {
              setView('playground');
              setTryCode(null); // Ensure modal is closed if navigating away
            }}
          >
            Playground
          </button>
        </nav>

        <div className="header-actions">
          {view === 'playground' && (
            <button className="run-button" onClick={handleRun} disabled={isRunning}>
              {isRunning ? 'Running...' : 'Run Code'}
            </button>
          )}
        </div>
      </header>
      
      <main className="main-area">
        {view === 'docs' ? (
          <Docs onTryCode={handleTryCode} />
        ) : (
          <div className="playground-container">
            <div className="editor-pane">
              <div className="pane-title">Code (Main Playground)</div>
              <Editor
                height="100%"
                defaultLanguage="c"
                theme="vs-dark"
                value={mainCode}
                onChange={(value) => setMainCode(value || '')}
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
        )}
      </main>

      {/* Try It Yourself Modal Overlay */}
      {tryCode !== null && (
        <div className="try-modal-overlay">
          <div className="try-modal-content">
            <div className="try-modal-header">
              <div className="try-modal-title">
                🍥 Try it Yourself
              </div>
              <div className="try-modal-actions">
                <button className="run-button" onClick={handleRun} disabled={isRunning} style={{ marginRight: '15px' }}>
                  {isRunning ? 'Running...' : 'Run Snippet'}
                </button>
                <button className="close-modal-btn" onClick={closeTryModal}>
                  &times; Close
                </button>
              </div>
            </div>
            <div className="try-modal-body">
              <div className="editor-pane">
                <div className="pane-title">Code Snippet</div>
                <Editor
                  height="100%"
                  defaultLanguage="c"
                  theme="vs-dark"
                  value={tryCode}
                  onChange={(value) => setTryCode(value || '')}
                  options={{
                    minimap: { enabled: false },
                    fontSize: 14,
                    fontFamily: '"Fira Code", monospace',
                  }}
                />
              </div>
              
              <div className="terminal-pane">
                <div className="pane-title">Execution Terminal</div>
                <div className="xterm-container" ref={terminalRef}></div>
              </div>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;
