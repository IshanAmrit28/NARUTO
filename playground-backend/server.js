const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

const app = express();
const port = process.env.PORT || 3001;

app.use(cors());
app.use(express.json());

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
    let narutoProcess = null;
    let tempFile = null;

    ws.on('message', (message) => {
        const data = JSON.parse(message);
        
        if (data.type === 'start') {
            // Write the code to a temporary file
            const tempDir = path.join(__dirname, 'temp');
            if (!fs.existsSync(tempDir)) {
                fs.mkdirSync(tempDir);
            }
            tempFile = path.join(tempDir, `script_${Date.now()}.nt`);
            fs.writeFileSync(tempFile, data.code);

            // Determine executable path (Linux/Render vs Local Windows)
            const localWinPath = path.join(__dirname, '..', 'naruto.exe');
            const linuxPath = path.join(__dirname, 'naruto');
            const exePath = fs.existsSync(linuxPath) ? linuxPath : localWinPath;

            try {
                narutoProcess = spawn(exePath, [tempFile]);

                narutoProcess.stdout.on('data', (output) => {
                    ws.send(JSON.stringify({ type: 'output', data: output.toString() }));
                });

                narutoProcess.stderr.on('data', (output) => {
                    ws.send(JSON.stringify({ type: 'output', data: output.toString() }));
                });

                narutoProcess.on('close', (code) => {
                    ws.send(JSON.stringify({ type: 'exit', code }));
                    if (fs.existsSync(tempFile)) {
                        fs.unlinkSync(tempFile);
                    }
                });

                narutoProcess.on('error', (err) => {
                    ws.send(JSON.stringify({ type: 'output', data: `\\r\\nFailed to start process: ${err.message}\\r\\n` }));
                });
            } catch (err) {
                ws.send(JSON.stringify({ type: 'output', data: `\\r\\nError: ${err.message}\\r\\n` }));
            }
        } else if (data.type === 'input') {
            if (narutoProcess && narutoProcess.stdin) {
                narutoProcess.stdin.write(data.input);
            }
        }
    });

    ws.on('close', () => {
        if (narutoProcess) {
            narutoProcess.kill();
        }
        if (tempFile && fs.existsSync(tempFile)) {
            fs.unlinkSync(tempFile);
        }
    });
});

app.get('/health', (req, res) => res.send('OK'));

// Serve the compiled React frontend if it exists (for Render deployment)
const distPath = path.join(__dirname, '..', 'playground', 'dist');
if (fs.existsSync(distPath)) {
    app.use(express.static(distPath));
    app.get('*', (req, res) => {
        res.sendFile(path.join(distPath, 'index.html'));
    });
}

server.listen(port, () => {
    console.log(`Naruto Playground backend listening on port ${port}`);
});
