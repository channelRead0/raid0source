#include <pch.h>
#include "networkmanager.hpp"

#include "sdk/internal/register/envregister.hpp"
#include "lock.hpp"

InboundHandlerTuplePacketListener::InboundHandlerTuplePacketListener(JNIEnv* Env)
{
	this->InboundHandlerTuplePacketListenerMapper = this->InboundHandlerTuplePacketListenerMapper ? this->InboundHandlerTuplePacketListenerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/NetworkManager$InboundHandlerTuplePacketListener");
}

InboundHandlerTuplePacketListener::InboundHandlerTuplePacketListener(JNIEnv* Env, jobject InstanceObject)
{
	this->InboundHandlerTuplePacketListenerMapper = this->InboundHandlerTuplePacketListenerMapper ? this->InboundHandlerTuplePacketListenerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/NetworkManager$InboundHandlerTuplePacketListener");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<InboundHandlerTuplePacketListener> InboundHandlerTuplePacketListener::New(JNIEnv* Env, jobject Packet) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<InboundHandlerTuplePacketListener>(Env, jobject());
	}

	const auto InitMethodID = Env->GetMethodID(InboundHandlerTuplePacketListenerMapper->GetClass(), "<init>", "(net/minecraft/network/Packet;[Lio/netty/util/concurrent/GenericFutureListener;)V");

	if (!InitMethodID) {
		return std::make_shared<InboundHandlerTuplePacketListener>(Env, jobject());
	}

	if (const auto NewInboundHandlerTuplePacketListener = Env->NewObject(InboundHandlerTuplePacketListenerMapper->GetClass(), InitMethodID, Packet, jobject())) {
		return std::make_shared<InboundHandlerTuplePacketListener>(Env, NewInboundHandlerTuplePacketListener);
	}

	return std::make_shared<InboundHandlerTuplePacketListener>(Env, jobject());
}

InboundHandlerTuplePacketListener::~InboundHandlerTuplePacketListener()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}

INetHandler::INetHandler(JNIEnv* Env)
{
}

INetHandler::INetHandler(JNIEnv* Env, jobject InstanceObject)
{
	this->InstanceObject = InstanceObject;
}

INetHandler::~INetHandler()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	}

	InstanceObject = nullptr;
}

NetworkManager::NetworkManager(JNIEnv* Env)
{
	this->NetworkManagerMapper = this->NetworkManagerMapper ? this->NetworkManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/NetworkManager");
}

NetworkManager::NetworkManager(JNIEnv* Env, jobject InstanceObject)
{
	this->NetworkManagerMapper = this->NetworkManagerMapper ? this->NetworkManagerMapper : std::make_shared<GameClass>(Env, "net/minecraft/network/NetworkManager");
	this->InstanceObject = InstanceObject;
}

std::shared_ptr<ReentrantReadWriteLock> NetworkManager::ReadWriteLock(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<ReentrantReadWriteLock>(Env, jobject());
	}

	if (const auto ReadWriteLockObject = NetworkManagerMapper->GetObjectField(Env, InstanceObject, "readWriteLock")) {
		return std::make_shared<ReentrantReadWriteLock>(Env, ReadWriteLockObject);
	}

	return std::make_shared<ReentrantReadWriteLock>(Env, jobject());
}

std::shared_ptr<Queue> NetworkManager::OutboundPacketsQueue(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<Queue>(Env, jobject());
	}

	if (const auto OutboundPacketsQueueObject = NetworkManagerMapper->GetObjectField(Env, InstanceObject, "outboundPacketsQueue")) {
		return std::make_shared<Queue>(Env, OutboundPacketsQueueObject);
	}

	return std::make_shared<Queue>(Env, jobject());
}

std::shared_ptr<INetHandler> NetworkManager::PacketListener(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return std::make_shared<INetHandler>(Env, jobject());
	}

	if (const auto PacketListenerObject = NetworkManagerMapper->GetObjectField(Env, InstanceObject, "packetListener")) {
		return std::make_shared<INetHandler>(Env, PacketListenerObject);
	}

	return std::make_shared<INetHandler>(Env, jobject());
}

jobject NetworkManager::GetNetHandler(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return jobject();
	}

	const auto NetHandlerMid = Env->GetMethodID(NetworkManagerMapper->GetClass(), "getNetHandler", "()Lnet/minecraft/network/INetHandler;");

	if (!NetHandlerMid) {
		return jobject();
	}

	return Env->CallObjectMethod(InstanceObject, NetHandlerMid);
}

bool NetworkManager::IsChannelOpen(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return{};
	}

	return NetworkManagerMapper->CallBooleanMethod(Env, InstanceObject, "isChannelOpen");
}

void NetworkManager::FlushOutboundQueue(JNIEnv* Env) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	NetworkManagerMapper->CallVoidMethod(Env, InstanceObject, "flushOutboundQueue");
}

void NetworkManager::ChannelRead0(JNIEnv* Env, const jobject ChannelHandlerContextObject, const jobject PacketObject) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	NetworkManagerMapper->CallVoidMethod(Env, InstanceObject, "channelRead0", ChannelHandlerContextObject, PacketObject);
}

//NO EVENTS
void NetworkManager::DispatchPacketNoEvent(JNIEnv* Env, const jobject PacketObject) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	NetworkManagerMapper->CallVoidMethod(Env, InstanceObject, "dispatchPacket", PacketObject, jobject());
}

void NetworkManager::SendPacket(JNIEnv* Env, const jobject PacketObject) const
{
	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	const auto SendPacketMid = Env->GetMethodID(NetworkManagerMapper->GetClass(), "sendPacket", "(Lnet/minecraft/network/Packet;)V");

	Env->CallVoidMethod(InstanceObject, SendPacketMid, PacketObject);
}

void NetworkManager::SendPacketWithoutEvent(JNIEnv* Env, const jobject PacketObject) const
{
	//
	//public void sendPacketWithoutEvent(Packet packetIn) {
	//
	//	if (this.isChannelOpen()) {
	//		this.flushOutboundQueue();
	//		this.dispatchPacket(packetIn, (GenericFutureListener< ? extends Future< ? super Void>>[]) null);
	//	}
	//	else {
	//		this.field_181680_j.writeLock().lock();
	//
	//		try {
	//			this.outboundPacketsQueue.add(new NetworkManager.InboundHandlerTuplePacketListener(packetIn, (GenericFutureListener[]) null));
	//		}
	//		finally {
	//			this.field_181680_j.writeLock().unlock();
	//		}
	//	}
	//
	//}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	if (IsChannelOpen(Env))
	{
		this->FlushOutboundQueue(Env);
		this->DispatchPacketNoEvent(Env, PacketObject);
	}
	else
	{
		this->ReadWriteLock(Env)->WriteLock(Env)->LockT(Env);

		const auto NewInboundHandlerTuplePacketListener = std::make_shared<InboundHandlerTuplePacketListener>(Env)->New(Env, PacketObject);
		this->OutboundPacketsQueue(Env)->Add(Env, NewInboundHandlerTuplePacketListener->GetInstanceObject());

		if (Env->ExceptionCheck()) 
		{
			Env->ExceptionDescribe();
			Env->ExceptionClear();
		}

		this->ReadWriteLock(Env)->WriteLock(Env)->UnlockT(Env);
	}
}

void NetworkManager::ReadPacketWithoutEvent(JNIEnv* Env, const std::shared_ptr<Packet> PacketInstance) const
{
	//public void receivePacketWithoutEvent(Packet packet) {
	//	if (this.channel.isOpen()) {
	//		try {
	//			packet.processPacket(this.packetListener);
	//		}
	//		catch (ThreadQuickExitException var4) {
	//			;
	//		}
	//	}
	//}

	if (Env->IsSameObject(InstanceObject, NULL)) {
		return;
	}

	if (IsChannelOpen(Env))
	{
		//packet.processPacket(this.packetListener);
		PacketInstance->ProcessPacket(Env, this->PacketListener(Env)->GetInstanceObject());

		if (Env->ExceptionCheck())
		{
			Env->ExceptionDescribe();
			Env->ExceptionClear();
		}
	}
}

NetworkManager::~NetworkManager()
{
	const auto Env = EnvRegister::GetThreadEnv();

	if (Env == nullptr) {
		return;
	}

	if (InstanceObject && !Env->IsSameObject(InstanceObject, NULL))
	{
		const auto ReferenceType = Env->GetObjectRefType(InstanceObject);

		if (ReferenceType == JNIGlobalRefType || ReferenceType == JNIWeakGlobalRefType) { Env->DeleteGlobalRef(InstanceObject); }
		if (ReferenceType == JNILocalRefType) { Env->DeleteLocalRef(InstanceObject); }
	//	Env->DeleteGlobalRef(InstanceObject);
	}

	InstanceObject = nullptr;
}
