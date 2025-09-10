import { Component } from '../Component';

export class Transform extends Component {
    public x: number;
    public y: number;
    public z: number;
    public rotation: number;
    public scale: number;

    constructor(x = 0, y = 0, z = 0, rotation = 0, scale = 1) {
        super();
        this.x = x;
        this.y = y;
        this.z = z;
        this.rotation = rotation;
        this.scale = scale;
    }

    public clone(): Transform {
        return new Transform(this.x, this.y, this.z, this.rotation, this.scale);
    }
}