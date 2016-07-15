#include"MsgDispatcher.h"
void MsgDispatcher::discharge(VirtualEntity* receiver, const Telegram& tel)
{
	if (!receiver->handleMsg(tel))
	{
		assert(0 && "Msg missing");
	}
}

void MsgDispatcher::addMsg(long int delay, int sender, int receiver, int msg, void* extraInfo)
{
	auto E_receiver = EMGR->getEntityByID(receiver);
	auto Msg = MsgType(msg);

	assert(E_receiver&&"recevier cannot be found");

	Telegram tel(clock(), sender, receiver, Msg, extraInfo);
	
	if (delay==0)
	{ }
	else
	{
		long int currentTime = clock();
		tel.dispatchTime_ = delay + currentTime;
	}

	this->_msgQueue.insert(tel);
}

void MsgDispatcher::dispatchMsg()
{
	while (!_msgQueue.empty() &&
		(_msgQueue.begin()->dispatchTime_ < long(clock())) &&
		(_msgQueue.begin()->dispatchTime_ > 0))
	{
		//read the telegram from the front of the queue
		const Telegram& telegram = *_msgQueue.begin();

		//find the recipient
		VirtualEntity* receiver = EMGR->getEntityByID(telegram.receiver_);

		/*cout << "\nQueued telegram ready for dispatch: Sent to "
			<< EMGR->getNameOfEntity(receiver->ID()) << ". Msg is " << Msg2Str(telegram.msg_);*/

		//send the telegram to the recipient
		discharge(receiver, telegram);

		//remove it from the queue
		_msgQueue.erase(_msgQueue.begin());
	}
}