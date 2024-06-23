#pragma once
#include "src/sdk/internal/templates/classbase.hpp"
#include "queue.hpp"
#include "packet.hpp"
#include "lock.hpp"

class InboundHandlerTuplePacketListener : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> InboundHandlerTuplePacketListenerMapper = nullptr;
public:
	InboundHandlerTuplePacketListener(JNIEnv* Env);
	InboundHandlerTuplePacketListener(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<InboundHandlerTuplePacketListener> New(JNIEnv* Env, jobject Packet) const;

	~InboundHandlerTuplePacketListener();
};

class INetHandler : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> INetHandlerMapper = nullptr;
public:
	INetHandler(JNIEnv* Env);
	INetHandler(JNIEnv* Env, jobject InstanceObject);

	~INetHandler();
};

class NetworkManager : public ClassBase
{
private:
	inline static std::shared_ptr<GameClass> NetworkManagerMapper = nullptr;
public:
	NetworkManager(JNIEnv* Env);
	NetworkManager(JNIEnv* Env, jobject InstanceObject);

	std::shared_ptr<Queue> OutboundPacketsQueue(JNIEnv* Env) const;
	std::shared_ptr<ReentrantReadWriteLock> ReadWriteLock(JNIEnv* Env) const;
	std::shared_ptr<INetHandler> PacketListener(JNIEnv* Env) const;

	jobject GetNetHandler(JNIEnv* Env) const;

	bool IsChannelOpen(JNIEnv* Env) const;
	void FlushOutboundQueue(JNIEnv* Env) const;
	void ChannelRead0(JNIEnv* Env, jobject ChannelHandlerContextObject, jobject PacketObject) const;
	void DispatchPacketNoEvent(JNIEnv* Env, const jobject PacketObject) const;

	void SendPacket(JNIEnv* Env, const jobject PacketObject) const;
	void SendPacketWithoutEvent(JNIEnv* Env, const jobject PacketObject) const;
	void ReadPacketWithoutEvent(JNIEnv* Env, const std::shared_ptr<Packet> PacketInstance) const;

	~NetworkManager();
};
