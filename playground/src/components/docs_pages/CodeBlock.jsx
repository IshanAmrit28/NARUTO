import React from 'react';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { vscDarkPlus } from 'react-syntax-highlighter/dist/esm/styles/prism';

export default function CodeBlock({ code, onTryCode, runnable = true }) {
  // Trim trailing newlines
  const codeString = code.replace(/\n$/, '');

  return (
    <div className="code-block-wrapper">
      <div className="code-block-header">
        <span className="code-lang">NARUTO</span>
        {runnable && (
          <button 
            className="try-btn" 
            onClick={() => onTryCode(codeString)}
          >
            Try it Yourself &raquo;
          </button>
        )}
      </div>
      <SyntaxHighlighter
        style={vscDarkPlus}
        language="cpp" // Under the hood, use cpp for syntax highlighting
        PreTag="div"
        className="syntax-highlighter-pre"
      >
        {codeString}
      </SyntaxHighlighter>
    </div>
  );
}
