import { Component } from '../Component';

export class Velocity extends Component {
    public dx: number;
    public dy: number;
    public dz: number;

    constructor(dx = 0, dy = 0, dz = 0) {
        super();
        this.dx = dx;
        this.dy = dy;
        this.dz = dz;
    }

    public clone(): Velocity {
        return new Velocity(this.dx, this.dy, this.dz);
    }
}