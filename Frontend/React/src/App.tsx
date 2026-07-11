import "@chatscope/chat-ui-kit-styles/dist/default/styles.min.css";
import {
  MainContainer,
  ChatContainer,
  MessageList,
  Message,
  MessageInput,
  Avatar
} from "@chatscope/chat-ui-kit-react";
import { useState, useEffect } from "react";
import mqtt from "mqtt";

type ChatMessage = {
  id: string;
  sender: string;
  header: string;
  avatar: string;
  text: string;
  direction: "incoming" | "outgoing";
};

type SourceType = "server" | "agent" | "mcu" | "system" | "self";

const avatars: Record<SourceType, string> = {
  server: "🖥️",
  agent: "🤖",
  mcu: "📟",
  system: "⌨️",
  self: "🧑"
};

const labels: Record<SourceType, string> = {
  server: "Server",
  agent: "Agent",
  mcu: "MCU",
  system: "System",
  self: "Self"
};

function getSource(sender: string): SourceType {
  if (sender.startsWith("agent")) return "agent";
  if (sender.startsWith("mcu")) return "mcu";
  if (sender.startsWith("server")) return "server";
  if (sender.startsWith("system")) return "system";
  return "self"; // fallback
}

function systemMessage(sysText: string): ChatMessage {
  return {
      id: "sys",
      sender: "system",
      header: "System",
      avatar: "⌨️",
      text: sysText,
      direction: "incoming"
  }
}
function mapSource(source: SourceType) {
  return {label:labels[source],avatar:avatars[source]};
}

function mqttToMes(payload: string): ChatMessage {
  const jsonMsg = JSON.parse(payload);
  const source = getSource(jsonMsg.sender)
  const identity = mapSource(source);
  return{
    id: jsonMsg.id,
    sender: source,
    header: identity.label,
    avatar: identity.avatar,
    text: jsonMsg.text,
    direction: "incoming"
  }
}

export default function App() {
  const client = mqtt.connect("ws://localhost:9001");
  const TOPICS = {
  AGENT: "agent/pub",
  UI: "ui/pub",
  MCU: "mcu/pub",
  GENERAL: "gen"
};
  useEffect(() => {
    
    
    client.on("reconnect", () => {
      console.log("Reconnecting...");
    });

    client.on("close", () => {
      console.log("Connection closed");
    });

    client.on("connect", () => {
      setMessages(prev => [...prev, systemMessage("connected to broker.")]);
      client.subscribe(TOPICS.AGENT);
      client.subscribe(TOPICS.MCU);

      client.publish(
        TOPICS.UI,
        JSON.stringify({
            id: "ui1",
            sender: "ui",
            text: "React UI connected to broker",
        })
      );
    });

    client.on("message", (topic, payload) => {
      console.log(topic.toString(), payload.toString());
    const msg = mqttToMes(payload.toString())
    
    setMessages(prev => [...prev, msg]);
  });
    


  }, []);
  const [messages, setMessages] = useState<ChatMessage[]>([
    {
      id: "sys",
      sender: "system",
      header: "System",
      avatar: "⌨️",
      text: "System Started.",
      direction: "incoming"
    },
  ]);
  
  function sendMessage(msg: string){
      const selfMsg = JSON.stringify({
            id: "ui1",
            sender: "ui",
            text: msg,
      })
      const selfMsgString = mqttToMes(selfMsg.toString())
    
      setMessages(prev => [...prev, selfMsgString]);
      client.publish(
        TOPICS.UI,
        JSON.stringify({
            id: "ui1",
            sender: "ui",
            text: msg,
        })
      );
  }
  return (
    <div style={{ height: "100vh" }}>
      <MainContainer>
        <ChatContainer>
          <MessageList>
             {messages.map(msg => (
                <Message
                  key={msg.id}
                  model={{
                    message: msg.text,
                    direction: msg.direction,
                    position: "single",
                  }}
                  className={`msg-${msg.sender}`}
                >
                  <Avatar name={msg.avatar} />
                  <Message.Header>{msg.header}</Message.Header>
                </Message>
              ))}
          </MessageList>
          <MessageInput placeholder="Type a message..."
        onSend={(msg) => {sendMessage(msg)}}/>
        </ChatContainer>
      </MainContainer>
    </div>
  );
}