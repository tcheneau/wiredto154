#ifndef WIREDTO154_MODELS_PATHLOSSMODEL_H
#define WIREDTO154_MODELS_PATHLOSSMODEL_H

class PathLossModel {
	public:
	virtual bool receivePacket(Coordinate X, Coordinate Y)=0;
};


class LogNormalShadowing: public PathLossModel {
	float path_loss; /* (Greek letter eta) */
	float shadowing_deviation; /* (Greek letter sigma) */
	float path_loss_d0; /* Path loss at distance 0 */
	float background_noise;

	public:
		LogNormalShadowing(Environment env);
		virtual bool receivePacket(Coordinate X, Cordinate Y);
}

#endif /* WIREDTO154_MODELS_PATHLOSSMODEL_H */
